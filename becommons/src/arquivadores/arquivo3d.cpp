#include "arquivo3d.hpp"
#include "src/depuracao/debug.hpp"
#include "iostream"
#include "filesystem"
#include "utility"
#include "imageloader.hpp"
#include <glm/gtc/type_ptr.hpp>

using namespace Bubble::Arquivadores;

std::vector<std::pair<Node, std::string>> arquivos;

static std::size_t gerarId(const std::string& str) {
    std::hash<std::string> hash_fn; // Função de hash para string
    return hash_fn(str);            // Gera o hash
}

Arquivo3d::Arquivo3d() {}

Arquivo3d::Arquivo3d(const std::string& caminho) : PathCompleto(caminho) {
    carregar();
}

Arquivo3d::Arquivo3d(const std::wstring& caminho) : PathCompleto(std::filesystem::path(caminho).string()) {
    carregar();
}


std::vector<Textura> Arquivo3d::processarTextura(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
    std::vector<Textura> textures;
    auto& gerenciadorTexturas = TextureLoader::getInstance();

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        Textura texture{};

        std::string filename = std::filesystem::path(str.C_Str()).filename().string();
        Debug::emitir("Importer", "Texture name: " + filename);

        // Verifica textura interna
        if (str.C_Str()[0] == '*') {
            int textureIndex = std::atoi(str.C_Str() + 1);
            const aiTexture* texture_ = cena->mTextures[textureIndex];

            texture.ID = gerenciadorTexturas.carregarAiTexture(texture_);
        }
        else {
            // Carrega textura usando o gerenciador de texturas.
            texture.ID = gerenciadorTexturas.carregarTextura(str.C_Str());
        }

        texture.tipo = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
    }
    return textures;
}


void Arquivo3d::carregar() {
    for (const auto& arquivo : arquivos) {      // Re-utiliza se já carregado
        if (arquivo.second == PathCompleto) {
            Debug::emitir(Debug::Alerta, "Arquivo 3D já existente, re-utilizando");
            RootNode = arquivo.first;
            Caminho = arquivo.second;
            return;
        }
    }

    if(foi_carregado)
    {
        Debug::emitir("Importer", "Já foi carregado");
        return;
    }
    Assimp::Importer importador;
    cena = importador.ReadFile(PathCompleto, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);

    if (!cena || (cena->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !cena->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importador.GetErrorString() << std::endl;
        return;
    }

    Caminho = std::filesystem::path(PathCompleto).parent_path().string();

    RootNode = processarNos(cena->mRootNode, 0); // Adiciona rootnode com seus filhos
    arquivos.push_back(std::make_pair(RootNode, PathCompleto));
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
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            malha.vertices.push_back(mesh->mVertices[j].x);
            malha.vertices.push_back(mesh->mVertices[j].y);
            malha.vertices.push_back(mesh->mVertices[j].z);
        }

        // Extrair índices
        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            const aiFace& face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                malha.indices.push_back(face.mIndices[k]);
            }
        }

        // Extrair UVs
        if (mesh->mTextureCoords[0]) {
            for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
                malha.uvs.push_back(mesh->mTextureCoords[0][j].x);
                malha.uvs.push_back(mesh->mTextureCoords[0][j].y);
            }
        }

        // Extrair normais
        if (mesh->HasNormals()) {
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

    aiColor3D corEspecular;
    if (material->Get(AI_MATKEY_COLOR_SPECULAR, corEspecular) == AI_SUCCESS) {
        mat.especular = { corEspecular.r, corEspecular.g, corEspecular.b };
    }
    else {
        mat.especular = { 0.7f, 0.7f, 0.7f }; // Cor padrão se não encontrada
    }

    // Obter o brilho (shininess)
    float shininess;
    if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
        mat.shininess = shininess;
    }
    else {
        mat.shininess = 32.0f; // Valor padrão para shininess se não encontrado
    }

    // Obter a refletividade (reflexão)
    float reflectivity;
    if (material->Get(AI_MATKEY_REFLECTIVITY, reflectivity) == AI_SUCCESS) {
        mat.reflexao = reflectivity;
    }
    else {
        mat.reflexao = 0.0f; // Valor padrão de reflexão se não encontrado
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

    mat.ID = gerarId(mat.nome);

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