#include "arquivo3d.hpp"
#include "src/depuracao/debug.hpp"
#include "iostream"
#include "filesystem"
#include "utility"
#include "imageloader.hpp"
#include <glm/gtc/type_ptr.hpp>

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

        std::string filename = std::filesystem::path(str.C_Str()).filename().string();
        Debug::emitir("Importer", "Texture name: " + filename);

        if (str.C_Str()[0] == '*') {
            int textureIndex = std::atoi(str.C_Str() + 1);
            const aiTexture* texture_ = cena->mTextures[textureIndex];

            if (texture_) {
                // Verifica se é uma imagem comprimida no formato JPEG
                FIMEMORY* fiMemory = FreeImage_OpenMemory(reinterpret_cast<BYTE*>(texture_->pcData), texture_->mWidth);
                FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(fiMemory);

                if (format != FIF_UNKNOWN) {
                    FIBITMAP* dib = FreeImage_LoadFromMemory(format, fiMemory);
                    if (dib) {
                        BYTE* data = FreeImage_GetBits(dib);
                        unsigned int width = FreeImage_GetWidth(dib);
                        unsigned int height = FreeImage_GetHeight(dib);
                        unsigned int bpp = FreeImage_GetBPP(dib);
                        int numChannels = bpp / 8; // Número de canais

                        texture.ID = TextureFromFile(data, width, height, numChannels);
                        FreeImage_Unload(dib); // Libera a memória usada pelo FIBITMAP
                    }
                    else {
                        Debug::emitir("Error", "Failed to load texture from memory.");
                    }
                }
                FreeImage_CloseMemory(fiMemory); // Libera a memória usada pelo FIMEMORY
            }
        }
        else {
            texture.ID = TextureFromFile(filename.c_str(), Caminho);  // Certifique-se de que "Caminho" esteja definido.
        }

        texture.tipo = typeName;
        texture.path = filename.c_str();  // Armazena a std::string diretamente.
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
    cena = importador.ReadFile(caminho, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

    if (!cena || (cena->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !cena->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importador.GetErrorString() << std::endl;
        return;
    }

    Caminho = std::filesystem::path(caminho).parent_path().string();

    RootNode = processarNos(cena->mRootNode, 0); // Adiciona rootnode com seus filhos
    arquivos.push_back(std::make_pair(RootNode, caminho));
}

glm::mat4& ConvertToGLMMat4(const aiMatrix4x4& aiMat) {
    glm::mat4 glmMat;

    glmMat[0][0] = aiMat.a1; glmMat[1][0] = aiMat.a2; glmMat[2][0] = aiMat.a3; glmMat[3][0] = aiMat.a4;
    glmMat[0][1] = aiMat.b1; glmMat[1][1] = aiMat.b2; glmMat[2][1] = aiMat.b3; glmMat[3][1] = aiMat.b4;
    glmMat[0][2] = aiMat.c1; glmMat[1][2] = aiMat.c2; glmMat[2][2] = aiMat.c3; glmMat[3][2] = aiMat.c4;
    glmMat[0][3] = aiMat.d1; glmMat[1][3] = aiMat.d2; glmMat[2][3] = aiMat.d3; glmMat[3][3] = aiMat.d4;

    return glmMat;
}

Node Arquivo3d::processarNos(aiNode* ai_node, unsigned int depth) {
    Node node_final;
    node_final.nome = ai_node->mName.C_Str();

    // Extrair a matriz de transformação do aiNode
    aiMatrix4x4 aiTransform = ai_node->mTransformation;
    node_final.transformacao = ConvertToGLMMat4(aiTransform);

    for (unsigned int i = 0; i < ai_node->mNumMeshes; ++i) {
        aiMesh* mesh = cena->mMeshes[ai_node->mMeshes[i]];
        if (!mesh) continue;

        Vertex malha;
        malha.nome = mesh->mName.C_Str();
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
    // Lista de tipos de texturas a serem processadas
    std::vector<std::pair<aiTextureType, std::string>> tiposDeTexturas = {
        { aiTextureType_DIFFUSE, "textura_difusa" },
        { aiTextureType_SPECULAR, "textura_specular" },
        { aiTextureType_AMBIENT, "textura_ambient" },
        { aiTextureType_EMISSIVE, "textura_emissiva" },
        { aiTextureType_HEIGHT, "textura_height" },
        { aiTextureType_NORMALS, "textura_normal" },
        { aiTextureType_SHININESS, "textura_shininess" },
        { aiTextureType_OPACITY, "textura_opacity" },
        { aiTextureType_DISPLACEMENT, "textura_displacement" },
        { aiTextureType_LIGHTMAP, "textura_lightmap" },
        { aiTextureType_REFLECTION, "textura_reflection" }
    };

    // Processar todas as texturas do material
    for (const auto& tipo : tiposDeTexturas) {
        auto texturas = processarTextura(material, tipo.first, tipo.second);
        mat.texturas.insert(mat.texturas.end(), texturas.begin(), texturas.end());
    }

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