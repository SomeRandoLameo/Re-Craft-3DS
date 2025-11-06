#include "TextureArray.h"

#include <GL/glew.h>
#include <cmath>
#include <cstring>

namespace terra {
namespace assets {

struct Mipmap {
    unsigned char* data;
    std::size_t dimension;

    Mipmap(unsigned char* data, std::size_t dimension) : data(data), dimension(dimension) { }

    int Sample(std::size_t x, std::size_t y, std::size_t color_offset) {
        return data[(y * dimension + x) * 4 + color_offset];
    }
};

float GetColorGamma(int a, int b, int c, int d) {
    float an = a / 255.0f;
    float bn = b / 255.0f;
    float cn = c / 255.0f;
    float dn = d / 255.0f;

    return (std::pow(an, 2.2f) + std::pow(bn, 2.2f) + std::pow(cn, 2.2f) + std::pow(dn, 2.2f)) / 4.0f;
}

// Blend four samples into a final result after doing gamma conversions
int GammaBlend(int a, int b, int c, int d) {
    float result = std::pow(GetColorGamma(a, b, c, d), 1.0f / 2.2f);

    return static_cast<int>(255.0f * result);
}

// Performs basic pixel averaging filter for generating mipmap.
void BoxFilterMipmap(std::vector<unsigned char>& previous, std::vector<unsigned char>& data, std::size_t dim) {
    std::size_t size_per_tex = dim * dim * 4;
    std::size_t count = data.size() / size_per_tex;
    std::size_t prev_dim = dim * 2;

    unsigned int* pixel = (unsigned int*)data.data();
    for (std::size_t i = 0; i < count; ++i) {
        unsigned char* prev_tex = previous.data() + i * (prev_dim * prev_dim * 4);

        Mipmap source(prev_tex, prev_dim);

        for (std::size_t y = 0; y < dim; ++y) {
            for (std::size_t x = 0; x < dim; ++x) {
                int red, green, blue, alpha;

                const std::size_t red_index = 0;
                const std::size_t green_index = 1;
                const std::size_t blue_index = 2;
                const std::size_t alpha_index = 3;

                red = GammaBlend(source.Sample(x * 2, y * 2, red_index), source.Sample(x * 2 + 1, y * 2, red_index),
                    source.Sample(x * 2, y * 2 + 1, red_index), source.Sample(x * 2 + 1, y * 2 + 1, red_index));

                green = GammaBlend(source.Sample(x * 2, y * 2, green_index), source.Sample(x * 2 + 1, y * 2, green_index),
                    source.Sample(x * 2, y * 2 + 1, green_index), source.Sample(x * 2 + 1, y * 2 + 1, green_index));

                blue = GammaBlend(source.Sample(x * 2, y * 2, blue_index), source.Sample(x * 2 + 1, y * 2, blue_index),
                    source.Sample(x * 2, y * 2 + 1, blue_index), source.Sample(x * 2 + 1, y * 2 + 1, blue_index));

                alpha = GammaBlend(source.Sample(x * 2, y * 2, alpha_index), source.Sample(x * 2 + 1, y * 2, alpha_index),
                    source.Sample(x * 2, y * 2 + 1, alpha_index), source.Sample(x * 2 + 1, y * 2 + 1, alpha_index));

                // AA BB GG RR
                *pixel = ((alpha & 0xFF) << 24) | ((blue & 0xFF) << 16) | ((green & 0xFF) << 8) | (red & 0xFF);
                ++pixel;
            }
        }
    }
}

TextureArray::TextureArray() {
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_TextureId);

    memset(m_Transparency, 0, sizeof(m_Transparency));
}

TextureHandle TextureArray::Append(const std::string& filename, const std::string& texture) {
    auto iter = m_Textures.find(filename);

    if (iter != m_Textures.end()) {
        return iter->second;
    }

    TextureHandle handle = static_cast<TextureHandle>(m_Textures.size());
    
    m_Textures[filename] = handle;
    m_TextureData.insert(m_TextureData.end(), texture.c_str(), texture.c_str() + texture.size());

    for (std::size_t i = 4; i < texture.size(); i += 4) {
        char alpha = texture[i];

        if (texture[i] == 0) {
            m_Transparency[handle] = true;
            break;
        }
    }

    return handle;
}

void TextureArray::Generate() {
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_TextureId);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int levels = 5;

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, levels - 1);

    GLsizei size = static_cast<GLuint>(m_Textures.size());

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, levels, GL_RGBA8, 16, 16, size);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, 16, 16, size, GL_RGBA, GL_UNSIGNED_BYTE, &m_TextureData[0]);

    std::vector<unsigned char> mipmap_data(m_TextureData);

    GLsizei dim = 16;
    for (int i = 1; i < levels; ++i) {
        dim /= 2;

        if (dim < 1) dim = 1;

        std::size_t data_size = dim * dim * 4 * size;
        std::vector<unsigned char> previous(mipmap_data);
        mipmap_data = std::vector<unsigned char>(data_size);

        BoxFilterMipmap(previous, mipmap_data, dim);

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, i, 0, 0, 0, dim, dim, size, GL_RGBA, GL_UNSIGNED_BYTE, mipmap_data.data());
    }

    m_TextureData.clear();
    std::vector<unsigned char>().swap(m_TextureData);
}

void TextureArray::Bind() {
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_TextureId);
}

bool TextureArray::GetTexture(const std::string& filename, TextureHandle* handle) {
    auto iter = m_Textures.find(filename);

    if (iter == m_Textures.end()) {
        return false;
    }

    *handle = iter->second;
    return true;
}

bool TextureArray::IsTransparent(TextureHandle handle) const {
    return m_Transparency[handle];
}

} // ns assets
} // ns terra
