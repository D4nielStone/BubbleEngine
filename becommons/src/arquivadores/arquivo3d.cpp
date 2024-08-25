#include "arquivo3d.hpp"
#include "src/depuracao/debug.hpp"
#include "iostream"
#include "filesystem"
#include "utility"
#include "imageloader.hpp"

using namespace Bubble::Arquivadores;

std::vector<std::pair<Node, std::string>> arquivos;

Arquivo3d::Arquivo3d() {}

Arquivo3d::Arquivo3d(const std::string& caminho) {
    carregar(caminho);
}

Arquivo3d::Arquivo3d(const std::wstring& caminho) {
    carregar(std::filesystem::path(caminho).string());
}

std::vector<Textura> Arquivo3d::processarTextura(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
    std::vector<Textura> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        Textura texture;

        if (str.C_Str()[0] == '*') {
            int textureIndex = std::atoi(str.C_Str() + 1);
            const aiTexture* texture_ = cena->mTextures[textureIndex];

            if (texture_) {
                unsigned char* data = reinterpret_cast<unsigned char*>(texture_->pcData);
                unsigned int width = texture_->mWidth;
                unsigned int height = texture_->mHeight;
                texture.ID = TextureFromFile(data, width, height);
            }
        }
        else {
            texture.ID = TextureFromFile(str.C_Str(), Caminho);
        }
        texture.tipo = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
    }
    return textures;
}

void Arquivo3d::carregar(const std::string& caminho) {
    for (const auto& arquivo : arquivos) {      // Re-utiliza se já carregado
        if (arquivo.second == caminho) {
            Debug::emitir(Debug::Alerta, "Arquivo 3D já existente, re-utilizando");
            RootNode = arquivo.first;
            Caminho = arquivo.second;
            return;
        }
    }

    Assimp::Importer importador;
    cena = importador.ReadFile(caminho, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipUVs);

    if (!cena || (cena->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !cena->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importador.GetErrorString() << std::endl;
        return;
    }

    Caminho = std::filesystem::path(caminho).parent_path().string();

    RootNode = processarNos(cena->mRootNode, 0); // Adiciona rootnode com seus filhos
    arquivos.push_back(std::make_pair(RootNode, caminho));
}
Node Arquivo3d::processarNos(aiNode* ai_node, unsigned int depth) {
    Node node_final;
    node_final.nome = ai_node->mName.C_Str();

    for (unsigned int i = 0; i < ai_node->mNumMeshes; ++i) {
        aiMesh* mesh = cena->mMeshes[ai_node->mMeshes[i]];
        if (!mesh) continue;

        Vertex malha;

        // Extrair vértices
        malha.vertices.reserve(mesh->mNumVertices * 3);  // Reservar espaço para os vértices
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            malha.vertices.push_back(mesh->mVertices[j].x);
            malha.vertices.push_back(mesh->mVertices[j].y);
            malha.vertices.push_back(mesh->mVertices[j].z);
        }

        // Extrair índices
        malha.indices.reserve(mesh->mNumFaces * 3);  // Reservar espaço para os índices
        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            const aiFace& face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                malha.indices.push_back(face.mIndices[k]);
            }
        }

        // Extrair UVs
        if (mesh->mTextureCoords[0]) {
            malha.uvs.reserve(mesh->mNumVertices * 2);  // Reservar espaço para UVs
            for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
                malha.uvs.push_back(mesh->mTextureCoords[0][j].x);
                malha.uvs.push_back(mesh->mTextureCoords[0][j].y);
            }
        }

        // Extrair normais
        if (mesh->HasNormals()) {
            malha.normals.reserve(mesh->mNumVertices * 3);  // Reservar espaço para normais
            for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
                malha.normals.push_back(mesh->mNormals[j].x);
                malha.normals.push_back(mesh->mNormals[j].y);
                malha.normals.push_back(mesh->mNormals[j].z);
            }
        }

        malha.material = processarMateriais(cena->mMaterials[mesh->mMaterialIndex]);
        node_final.malhas.push_back(malha);
    }

    for (unsigned int i = 0; i < ai_node->mNumChildren; i++) {
        node_final.filhos.push_back(processarNos(ai_node->mChildren[i], depth+2));
    }
    
    Debug::emitir("Importer", "Node: " + std::string(depth, ' ') + node_final.nome);
    return node_final;
}

Material Arquivo3d::processarMateriais(aiMaterial* material) {
    Material mat;
    mat.nome = material->GetName().C_Str();

    aiColor3D corDifusa;
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, corDifusa) == AI_SUCCESS) {
        mat.difusa = { corDifusa.r, corDifusa.g, corDifusa.b };
    }
    else {
        mat.difusa = { 0.7f, 0.7f, 0.7f }; // Cor padrão se não encontrada
    }

    // Processar outras propriedades do material, se necessário
    return mat;
}

std::string Arquivo3d::nome() const {
    if (cena && cena->mRootNode) {
        return cena->mRootNode->mName.C_Str();
    }
    else {
        return "Cena sem nome ou inválida";
    }
}