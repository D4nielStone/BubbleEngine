#pragma once

#include <string>
#include <vector>
#include "includes.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

class ObjectFile
{
public:
    std::string Path;
    std::vector<Vertex> vertices;
    std::vector<Material> materials;

    ObjectFile(std::string path) : Path(path) {
        loadModel(Path);
    }

    void printModelInfo() {
        const char* nameptr = scene->mName.C_Str();
        std::cout << "Object file extension: " << getFileExtension(Path) << std::endl;
        std::cout << "Scene name: " << nameptr << std::endl;
        std::cout << ">> Number of meshes: " << scene->mNumMeshes << std::endl;
        for (size_t i = 0; i < vertices.size(); ++i) {
            std::cout << ">> Mesh " << i << ":\n";
            std::cout << "    Number of vertices: " << vertices[i].vertices.size() / 3 << std::endl;
            std::cout << "    Number of indices: " << vertices[i].indices.size() << std::endl;
            std::cout << "    Number of UV coordinates: " << vertices[i].uvs.size() / 2 << std::endl;
            std::cout << "    Number of Normals:  " << vertices[i].normals.size() / 3 << std::endl;
        }
    }

private:
    const aiScene* scene;

    void loadModel(const std::string& path) {
        Assimp::Importer importer;
        scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        extractData();
    }

    void extractData() {
        if (!scene) return;

        vertices.resize(scene->mNumMeshes);

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[i];

            Vertex vertexData;

            // Extract vertices
            for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
                vertexData.vertices.push_back(mesh->mVertices[j].x);
                vertexData.vertices.push_back(mesh->mVertices[j].y);
                vertexData.vertices.push_back(mesh->mVertices[j].z);
            }

            // Extract indices
            for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
                aiFace face = mesh->mFaces[j];
                for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                    vertexData.indices.push_back(face.mIndices[k]);
                }
            }

            // Extract UVs
            if (mesh->mTextureCoords[0]) {
                for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
                    vertexData.uvs.push_back(mesh->mTextureCoords[0][j].x);
                    vertexData.uvs.push_back(mesh->mTextureCoords[0][j].y);
                }
            }

            // Extract normals
            if (mesh->HasNormals()) {
                for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
                    vertexData.normals.push_back(mesh->mNormals[j].x);
                    vertexData.normals.push_back(mesh->mNormals[j].y);
                    vertexData.normals.push_back(mesh->mNormals[j].z);
                }
            }

            vertices[i] = vertexData;
        }

        extractMaterials();
    }

    void extractMaterials() {
        if (!scene) return;
        for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
            aiMaterial* material = scene->mMaterials[i];
            Material mat;

            aiString name;
            if (AI_SUCCESS == material->Get(AI_MATKEY_NAME, name)) {
                mat.name = name.C_Str();
            }

            aiColor3D color;
            if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, color)) {
                mat.ambient = { color.r, color.g, color.b };
            }

            if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
                mat.diffuse = { color.r, color.g, color.b };
            }

            if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, color)) {
                mat.specular = { color.r, color.g, color.b };
            }

            materials.push_back(mat);
        }
    }

    std::string getFileExtension(std::string& path) {
        size_t dotPosition = path.find_last_of('.');
        if (dotPosition == std::string::npos) {
            return "";
        }
        return path.substr(dotPosition + 1);
    }
};
