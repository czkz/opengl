#include "model_loader.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <Vector.h>
#include "VertexResolver.h"
#include "file_utils.h"
#include "GLScripts.h"
#include <dbg.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace {

    std::map<std::string, std::weak_ptr<Texture>> texture_cache;

    struct Vertex {
        Vector3 position;
        Vector3 normal;
        Vector2 texCoords;

        static size_t registerAttributes() {
            return VertexResolver::Register<Vector3, Vector3, Vector2>();
        }
    };

    std::vector<MeshEx::CachedTexture> loadMaterialTextures(aiMaterial *mat,
            std::string_view directory,
            aiTextureType type)
    {
        std::vector<MeshEx::CachedTexture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string fullPath = std::string(directory) + "/" + str.C_Str();

            std::shared_ptr<Texture> tex_ptr;
            auto cache_it = texture_cache.find(fullPath);
            if (cache_it == texture_cache.end()) {
                dp(fullPath);
                tex_ptr = std::make_shared<Texture>(make_texture(fullPath.c_str()));
                texture_cache[fullPath] = tex_ptr;
            } else {
                tex_ptr = cache_it->second.lock();
            }

            textures.push_back(std::move(tex_ptr));
        }
        return textures;
    }

    template <typename T1, typename T2>
    MeshEx make_mesh_ex(const T1& vertices, const T2& indices,
            std::vector<MeshEx::CachedTexture> textures_diffuse,
            std::vector<MeshEx::CachedTexture> textures_specular)
    {
        VAO vao;
        VBO vbo (vertices);
        EBO ebo (indices);
        vao.Bind();
        {
            vbo.Bind();
            ebo.Bind();
            size_t nAttrs = T1::value_type::registerAttributes();
            for (size_t i = 0; i < nAttrs; i++) {
                glEnableVertexAttribArray(i);
            }
        }
        VAO::Unbind();
        return MeshEx {
            std::move(vao), std::move(vbo), std::move(ebo),
            std::move(textures_diffuse), std::move(textures_specular)
        };
    }

    MeshEx processMesh(aiMesh* mesh, std::string_view directory, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
            /// TODO Assimp mesh can contain up to 8 texCoords
            if(mesh->mTextureCoords[0]) {
                vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
            } else {
                vertex.texCoords = {0, 0};
            }
            vertices.push_back(vertex);
        }
        for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            as(face.mNumIndices == 3);
            for(unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }
        // process material
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<MeshEx::CachedTexture> diffuseMaps = loadMaterialTextures(material,
                directory, aiTextureType_DIFFUSE);
        std::vector<MeshEx::CachedTexture> specularMaps = loadMaterialTextures(material,
                directory, aiTextureType_SPECULAR);

        return make_mesh_ex (
            std::move(vertices),
            std::move(indices),
            std::move(diffuseMaps),
            std::move(specularMaps)
        );
    }

}

namespace model_loader {

    std::vector<MeshEx> loadModel(std::string path) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path,
                aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene ||
                scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
                !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            throw 123;
        }
        if (path.empty() || path[0] != '/') { path = "./" + path; }
        std::string directory = path.substr(0, path.find_last_of('/'));

        // Not using Assimp tree structure, just parsing everything
        // processNode(scene->mRootNode, scene);
        std::vector<MeshEx> ret;
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            ret.push_back(processMesh(scene->mMeshes[i], directory, scene));
        }
        return ret;
    }
}
