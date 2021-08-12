#pragma once
#include <vector>
#include <string>
#include <memory>

#include "BufferObject.h"
#include "Texture.h"

struct MeshEx {
    using CachedTexture = std::shared_ptr<Texture>;
    VAO vao;
    VBO vbo;
    EBO ebo;
    std::vector<CachedTexture> textures_diffuse;
    std::vector<CachedTexture> textures_specular;
    std::vector<CachedTexture> textures_normals;
    size_t n_attributes = 0;
};

namespace model_loader {
    std::vector<MeshEx> loadModel(std::string path);
}
