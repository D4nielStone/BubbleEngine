#include "arquivo3d.h"

void Bubble::Arquivadores::Arquivo3d::exibirInformacoes() 
{
     const char* nameptr = cena->mName.C_Str();
     std::cout << "Extensão do arquivo: " << obterExtensaoArquivo(Caminho) << std::endl;
     std::cout << "Nome da cena: " << nameptr << std::endl;
     std::cout << ">> Número de malhas: " << cena->mNumMeshes << std::endl;
     for (size_t i = 0; i < vertices.size(); ++i) {
         std::cout << ">> Malha " << i << ":\n";
         std::cout << "    Número de vértices: " << vertices[i].vertices.size() / 3 << std::endl;
         std::cout << "    Número de índices: " << vertices[i].indices.size() << std::endl;
         std::cout << "    Número de coordenadas UV: " << vertices[i].uvs.size() / 2 << std::endl;
         std::cout << "    Número de normais: " << vertices[i].normals.size() / 3 << std::endl;
     }
}
void Bubble::Arquivadores::Arquivo3d::carregarModelo(const std::string& caminho) {
    Assimp::Importer importador;
    cena = importador.ReadFile(caminho, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!cena || cena->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !cena->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importador.GetErrorString() << std::endl;
        return;
    }

    extrairDados();
}
void Bubble::Arquivadores::Arquivo3d::extrairDados() {
    if (!cena) return;

    vertices.resize(cena->mNumMeshes);

    for (unsigned int i = 0; i < cena->mNumMeshes; ++i) {
        aiMesh* mesh = cena->mMeshes[i];

        Vertex dadosVertices;

        // Extrair vértices
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            dadosVertices.vertices.push_back(mesh->mVertices[j].x);
            dadosVertices.vertices.push_back(mesh->mVertices[j].y);
            dadosVertices.vertices.push_back(mesh->mVertices[j].z);
        }

        // Extrair índices
        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            aiFace face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                dadosVertices.indices.push_back(face.mIndices[k]);
            }
        }

        // Extrair UVs
        if (mesh->mTextureCoords[0]) {
            for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
                dadosVertices.uvs.push_back(mesh->mTextureCoords[0][j].x);
                dadosVertices.uvs.push_back(mesh->mTextureCoords[0][j].y);
            }
        }

        // Extrair normais
        if (mesh->HasNormals()) {
            for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
                dadosVertices.normals.push_back(mesh->mNormals[j].x);
                dadosVertices.normals.push_back(mesh->mNormals[j].y);
                dadosVertices.normals.push_back(mesh->mNormals[j].z);
            }
        }

        vertices[i] = dadosVertices;
    }

    extrairMateriais();
}
void Bubble::Arquivadores::Arquivo3d::extrairMateriais() {
    if (!cena) return;
    for (unsigned int i = 0; i < cena->mNumMaterials; ++i) {
        aiMaterial* material = cena->mMaterials[i];
        Material mat;

        aiString nome;
        if (AI_SUCCESS == material->Get(AI_MATKEY_NAME, nome)) {
            mat.nome = nome.C_Str();
        }

        aiColor3D cor;
        if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, cor)) {
            mat.ambiente = { cor.r, cor.g, cor.b };
        }

        if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, cor)) {
            mat.difusa = { cor.r, cor.g, cor.b };
        }

        if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, cor)) {
            mat.especular = { cor.r, cor.g, cor.b };
        }

        materiais.push_back(mat);
    }
}
std::string Bubble::Arquivadores::Arquivo3d::obterExtensaoArquivo(std::string& caminho) {
    size_t posPonto = caminho.find_last_of('.');
    if (posPonto == std::string::npos) {
        return "";
    }
    return caminho.substr(posPonto + 1);
}