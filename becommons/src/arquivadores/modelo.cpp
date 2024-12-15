#include "modelo.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <src/depuracao/debug.hpp>
#include "imageloader.hpp"
#include <filesystem>

std::vector<Textura> texturas_carregadas;

void Modelo::Desenhar(Shader& shader)
{
    for (unsigned int i = 0; i < malhas.size(); i++)
        malhas[i].desenhar(shader);
}

void Modelo::carregarModelo(const std::string& path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Debug::emitir(Erro, import.GetErrorString());
        return;
    }
    diretorio = path.substr(0, path.find_last_of('\\'));

    /// Processa o no principal
    processarNo(scene->mRootNode, scene);
}

void Modelo::processarNo(aiNode* node, const aiScene* scene)
{
    // processa todos as malhas se tiver
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* malha = scene->mMeshes[node->mMeshes[i]];
        malhas.push_back(processarMalha(malha, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processarNo(node->mChildren[i], scene);
    }

}

Malha Modelo::processarMalha(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertice> vertices;
    std::vector<unsigned int> indices;
    std::vector<Textura> texturas;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertice vertex;
        // processa coordenadas de vertice
        Vetor3<float> vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.posicao = vector;

        // normais
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        // uvs
        if (mesh->mTextureCoords[0])
        {
            Vetor2<float> vec;
             
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.uvcoords = vec;
        }
        else
            vertex.uvcoords = Vetor2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    /// processa os indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    
    // processa materiais
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    std::vector<Textura> diffuseMaps = carregarMaterialETexturas(material, aiTextureType_DIFFUSE, "texture_diffuse");
    texturas.insert(texturas.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Textura> specularMaps = carregarMaterialETexturas(material, aiTextureType_SPECULAR, "texture_specular");
    texturas.insert(texturas.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Textura> normalMaps = carregarMaterialETexturas(material, aiTextureType_HEIGHT, "texture_normal");
    texturas.insert(texturas.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Textura> heightMaps = carregarMaterialETexturas(material, aiTextureType_AMBIENT, "texture_height");
    texturas.insert(texturas.end(), heightMaps.begin(), heightMaps.end());

    /// extrai cor difusa
    aiColor4D diffuse_color;
    Cor difusa;
    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color))
    {
         difusa.r = diffuse_color.r;
         difusa.g = diffuse_color.g;
         difusa.b = diffuse_color.b;
         difusa.a = diffuse_color.a;
    }
    Material mat(texturas, difusa);

    return Malha(vertices, indices, mat);
}

std::vector<Textura> Modelo::carregarMaterialETexturas(aiMaterial* mat, aiTextureType type, std::string typeName) const
{
    std::vector<Textura> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < texturas_carregadas.size(); j++)
        {
            if (std::strcmp(texturas_carregadas[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(texturas_carregadas[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Textura texture;
            auto relativa = std::filesystem::path(str.C_Str()).filename().string();
            texture.id = TextureFromFile(this->diretorio +"\\"+ relativa);
            texture.tipo = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            texturas_carregadas.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}