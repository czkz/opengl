#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <Vector.h>
#include "model_data.h"
#include <dbg.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace assimp {
    struct Vertex {
        Vector3 position;
        Vector3 normal;
        Vector2 texCoords;

        static size_t registerAttributes() {
            return VertexResolver::Register<Vector3, Vector3, Vector2>();
        }
    };

    struct Texture {
        unsigned int id;
        std::string type;
    };

    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
    };

    Mesh processMesh(aiMesh* mesh, const aiScene*) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
            // Assimp mesh can contain up to 8 texCoords
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
        // if(mesh->mMaterialIndex >=??? 0) {
        // }

        return Mesh { vertices, indices, textures };
    }

    std::vector<Mesh> loadModel(std::string path) {
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

        // processNode(scene->mRootNode, scene);
        std::vector<Mesh> ret;
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            ret.push_back(processMesh(scene->mMeshes[i], scene));
        }
        return ret;
    }
}
