#include "AssetLoader.h"
#include "AssetCache.h"
#include "zip/ZipArchive.h"
#include "stb_image.h"
#include "../block/BlockModel.h"
#include "../block/BlockVariant.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <mclib/block/Block.h>
#include <mclib/common/Json.h>

namespace terra {
namespace assets {

std::string GetBlockNameFromPath(const std::string& path) {
    auto ext_pos = path.find_last_of('.');
    auto pos = path.find_last_of('/');
    auto filename = path.substr(pos + 1);
    auto ext_size = path.size() - ext_pos;

    return filename.substr(0, filename.length() - ext_size);
}

glm::vec3 GetVectorFromJson(const mc::json& node) {
    glm::vec3 result(0, 0, 0);

    int index = 0;

    for (auto& n : node) {
        if (n.is_number()) {
            result[index++] = n.get<glm::vec3::value_type>() / static_cast<glm::vec3::value_type>(16.0);
        }
    }

    return result;
}

std::pair<glm::vec2, glm::vec2> GetUVFromJson(const mc::json& node) {
    glm::vec2 from(0, 0);
    glm::vec2 to(1, 1);

    int index = 0;

    for (auto& n : node) {
        if (n.is_number()) {
            if (index < 2) {
                from[index++] = n.get<glm::vec3::value_type>() / static_cast<glm::vec3::value_type>(16.0);
            } else {
                to[index - 2] = n.get<glm::vec3::value_type>() / static_cast<glm::vec3::value_type>(16.0);
                ++index;
            }
        }
    }

    return std::make_pair<>(from, to);
}

AssetLoader::AssetLoader(AssetCache& cache) 
    : m_Cache(cache) 
{

}

bool AssetLoader::LoadArchive(const std::string& archive_path, const std::string& blocks_path) {
    if (!LoadBlockStates(blocks_path)) {
        return false;
    }

    terra::assets::ZipArchive archive;

    if (!archive.Open(archive_path.c_str())) {
        std::cerr << "Failed to open archive.\n";
        return false;
    }

    if (LoadBlockModels(archive) <= 0) {
        return false;
    }

    LoadBlockVariants(archive);

    m_Cache.GetTextures().Generate();

    // Fancy graphics mode seems to do something different than using cullface.
    for (auto& model : m_Cache.GetBlockModels("leaves")) {
        for (auto& element : model->GetElements()) {
            for (auto& face : element.GetFaces()) {
                face.cull_face = block::BlockFace::None;
                face.tint_index = 1;
            }
        }
    }

    return true;
}

bool AssetLoader::LoadBlockStates(const std::string& filename) {
    std::ifstream in(filename);

    if (!in) {
        std::cerr << "Failed to open " << filename << std::endl;
        return false;
    }

    std::string data;
    in.seekg(0, std::ios::end);
    std::size_t size = static_cast<std::size_t>(in.tellg());
    in.seekg(0, std::ios::beg);

    data.resize(size);

    in.read(&data[0], size);

    mc::json root;

    try {
        root = mc::json::parse(data);
    } catch (mc::json::parse_error& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    std::size_t max_id = 0;

    for (auto& kv : root.items()) {
        mc::json states_node = kv.value().value("states", mc::json());

        if (!states_node.is_array()) continue;

        for (auto& state_node : states_node) {
            u32 id = state_node.value("id", 0);

            if (id > max_id) {
                max_id = id;
            }
        }
    }

    m_Cache.SetMaxBlockId(max_id);

    for (auto& kv : root.items()) {
        std::string block_name = kv.key();
        mc::json states_node = kv.value().value("states", mc::json());

        if (!states_node.is_array()) continue;

        for (auto& state_node : states_node) {
            std::unique_ptr<terra::block::BlockState> state = terra::block::BlockState::FromJSON(state_node);

            m_Cache.AddBlockState(std::move(state));
        }
    }

    return true;
}

bool AssetLoader::LoadTexture(terra::assets::ZipArchive& archive, const std::string& path, TextureHandle* handle) {
    if (m_Cache.GetTextures().GetTexture(path, handle)) return true;

    std::string texture_path = "assets/minecraft/textures/" + path;

    int width, height, channels;
    std::size_t texture_size;
    char* texture_raw = archive.ReadFile(texture_path.c_str(), &texture_size);

    if (texture_raw == nullptr) return false;

    unsigned char* image = stbi_load_from_memory(reinterpret_cast<unsigned char*>(texture_raw), (int)texture_size, &width, &height, &channels, STBI_rgb_alpha);

    archive.FreeFileData(texture_raw);

    if (image == nullptr) return false;

    if (width == 16) {
        std::size_t size = 16 * 16 * 4;

        *handle = m_Cache.AddTexture(path, std::string(reinterpret_cast<char*>(image), size));
    }

    stbi_image_free(image);

    return true;
}

std::unique_ptr<block::BlockModel> AssetLoader::LoadBlockModel(terra::assets::ZipArchive& archive, const std::string& path, TextureMap& texture_map, std::vector<IntermediateElement>& intermediates) {
    std::string block_path = "block/" + GetBlockNameFromPath(path);

    size_t size;
    char* raw = archive.ReadFile(path.c_str(), &size);

    if (raw) {
        std::string contents(raw, size);

        archive.FreeFileData(raw);

        mc::json root;
        try {
            root = mc::json::parse(contents);
        } catch (mc::json::parse_error& e) {
            std::cerr << e.what() << std::endl;
            return nullptr;
        }

        auto parent_node = root.value("parent", mc::json());
        if (parent_node.is_string()) {
            std::string parent_path = "assets/minecraft/models/" + parent_node.get<std::string>() + ".json";

            // Load all of the texture mappings from the parent.
            LoadBlockModel(archive, parent_path, texture_map, intermediates);
        }

        auto model = std::make_unique<block::BlockModel>();

        if (model != nullptr) {
            // Load each texture listed as part of this model.
            auto textures_node = root.value("textures", mc::json());

            for (auto& kv : textures_node.items()) {
                std::string texture_path = kv.value().get<std::string>();

                texture_map[kv.key()] = texture_path;
            }

            auto elements_node = root.value("elements", mc::json());

            // Create each element of the model and resolve texture linking
            for (auto& element_node : elements_node) {
                glm::vec3 from = GetVectorFromJson(element_node.value("from", mc::json()));
                glm::vec3 to = GetVectorFromJson(element_node.value("to", mc::json()));

                IntermediateElement element;
                element.from = from;
                element.to = to;
                element.shade = element_node.value("shade", true);

                auto rotation_node = element_node.value("rotation", mc::json());

                if (rotation_node.is_object()) {
                    element.rotation.origin = GetVectorFromJson(rotation_node.value("origin", mc::json()));
                    std::string axis = rotation_node.value("axis", "y");

                    if (axis == "x") {
                        element.rotation.axis = glm::vec3(1, 0, 0);
                    } else if (axis == "y") {
                        element.rotation.axis = glm::vec3(0, 1, 0);
                    } else {
                        element.rotation.axis = glm::vec3(0, 0, 1);
                    }

                    element.rotation.angle = rotation_node.value("angle", 0.0f);
                    element.rotation.rescale = rotation_node.value("rescale", false);
                }

                auto faces_node = element_node.value("faces", mc::json());

                for (auto& kv : faces_node.items()) {
                    block::BlockFace face = block::face_from_string(kv.key());
                    auto face_node = kv.value();

                    block::BlockFace cull_face = block::face_from_string(face_node.value("cullface", ""));

                    auto texture_node = face_node.value("texture", mc::json());

                    if (texture_node.is_string()) {
                        std::string texture = texture_node.get<std::string>();

                        IntermediateFace renderable;

                        renderable.texture = texture;
                        renderable.face = face;
                        renderable.cull_face = cull_face;
                        renderable.tint_index = face_node.value("tintindex", -1);

                        std::tie(renderable.uv_from, renderable.uv_to) = GetUVFromJson(face_node.value("uv", mc::json()));
                        
                        element.faces.push_back(renderable);
                    }
                }

                intermediates.push_back(element);
            }

            return model;
        }
    }

    return nullptr;
}

std::size_t AssetLoader::LoadBlockModels(terra::assets::ZipArchive& archive) {
    std::size_t count = 0;

    for (std::string model_path : archive.ListFiles("assets/minecraft/models/block")) {
        TextureMap texture_map;
        std::vector<IntermediateElement> intermediates;

        auto model = LoadBlockModel(archive, model_path, texture_map, intermediates);

        // Resolve intermediate textures
        for (const IntermediateElement& intermediate : intermediates) {
            block::BlockElement element(intermediate.from, intermediate.to);

            element.SetShouldShade(intermediate.shade);
            element.GetRotation() = intermediate.rotation;

            for (const auto& intermediate_renderable : intermediate.faces) {
                std::string texture = intermediate_renderable.texture;

                while (!texture.empty() && texture[0] == '#') {
                    texture = texture_map[texture.substr(1)];
                }

                TextureHandle handle;
                if (LoadTexture(archive, texture + ".png", &handle)) {
                    block::RenderableFace renderable;

                    renderable.face = intermediate_renderable.face;
                    renderable.cull_face = intermediate_renderable.cull_face;
                    renderable.texture = handle;
                    renderable.tint_index = intermediate_renderable.tint_index;
                    renderable.uv_from = intermediate_renderable.uv_from;
                    renderable.uv_to = intermediate_renderable.uv_to;

                    element.AddFace(renderable);
                }
            }

            model->AddElement(element);
        }

        std::string block_path = "block/" + GetBlockNameFromPath(model_path);

        m_Cache.AddBlockModel(block_path, std::move(model));
        ++count;
    }

    return count;
}

void AssetLoader::LoadBlockVariants(terra::assets::ZipArchive& archive) {
    for (std::string block_state_path : archive.ListFiles("assets/minecraft/blockstates")) {
        size_t size;
        char* raw = archive.ReadFile(block_state_path.c_str(), &size);

        if (raw) {
            std::string contents(raw, size);

            mc::json root;
            try {
                root = mc::json::parse(contents);
            } catch (mc::json::parse_error& e) {
                std::cerr << e.what() << std::endl;
                continue;
            }

            mc::json variants_node = root.value("variants", mc::json());

            if (!variants_node.is_object()) continue;

            for (auto& kv : variants_node.items()) {
                std::string variant_name = kv.key();
                mc::json variant = kv.value();

                if (variant.is_array()) {
                    for (mc::json& variant_sub : variant) {
                        mc::json model_node = variant_sub.value("model", mc::json());
                        if (model_node.is_string()) {
                            std::string model_path = model_node.get<std::string>();

                            block::BlockModel* model = m_Cache.GetBlockModel(model_path);
                            
                            if (model == nullptr) {
                                std::cerr << "Could not find block model " << model_path << std::endl;
                                continue;
                            }

                            auto pos = block_state_path.find_last_of('/');
                            auto filename = block_state_path.substr(pos + 1);
                            auto state = filename.substr(0, filename.length() - 5);

                            mc::block::BlockPtr block = mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:" + state);

                            if (block == nullptr) {
                                std::cerr << "Could not find block minecraft:" << state << std::endl;
                                continue;
                            }

                            double x = variant_sub.value("x", 0);
                            double y = variant_sub.value("y", 0);
                            double z = variant_sub.value("z", 0);

                            std::unique_ptr<block::BlockVariant> variant = std::make_unique<block::BlockVariant>(model, variant_name, block);

                            variant->SetRotation(glm::vec3(x, y, z));

                            m_Cache.AddVariantModel(std::move(variant));
                        }
                    }
                } else {
                    mc::json model_node = variant.value("model", mc::json());
                    if (model_node.is_string()) {
                        std::string model_path = model_node.get<std::string>();

                        block::BlockModel* model = m_Cache.GetBlockModel(model_path);

                        if (model == nullptr) {
                            std::cerr << "Could not find block model " << model_path << std::endl;
                            continue;
                        }

                        auto pos = block_state_path.find_last_of('/');
                        auto filename = block_state_path.substr(pos + 1);
                        auto state = filename.substr(0, filename.length() - 5);

                        mc::block::BlockPtr block = mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:" + state);

                        if (block == nullptr) {
                            std::cerr << "Could not find block minecraft:" << state << std::endl;
                            continue;
                        }

                        double x = variant.value("x", 0);
                        double y = variant.value("y", 0);
                        double z = variant.value("z", 0);

                        std::unique_ptr<block::BlockVariant> variant = std::make_unique<block::BlockVariant>(model, variant_name, block);

                        variant->SetRotation(glm::vec3(x, y, z));

                        m_Cache.AddVariantModel(std::move(variant));
                    }
                }
            }
        }
    }
}

} // ns assets
} // ns terra
