#ifndef TERRACOTTA_ASSETS_TEXTUREARRAY_H_
#define TERRACOTTA_ASSETS_TEXTUREARRAY_H_

#include <cstddef>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace terra {
namespace assets {

using TextureHandle = uint32_t;

class TextureArray {
public:
    TextureArray();

    TextureHandle Append(const std::string& filename, const std::string& texture);

    bool GetTexture(const std::string& filename, TextureHandle* handle);
    bool IsTransparent(TextureHandle handle) const;

    void Generate();
    void Bind();
private:
    unsigned int m_TextureId;

    std::vector<unsigned char> m_TextureData;
    std::unordered_map<std::string, TextureHandle> m_Textures;

    bool m_Transparency[2048];
};

} // ns assets
} // ns terra

#endif
