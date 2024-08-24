#include "arquivo3d.hpp"
#include "src/depuracao/debug.hpp"
#include "iostream"
#include "filesystem"
#include "utility"

using namespace Bubble::Arquivadores;

std::vector<std::pair<std::pair<std::vector<Vertex>, std::vector<Material*>>, std::string>> arquivos;

Arquivo3d::Arquivo3d() {}
Arquivo3d::Arquivo3d(std::string caminho) : Caminho(caminho)
{
    carregarModelo(Caminho);
}

Bubble::Arquivadores::Arquivo3d::Arquivo3d(std::wstring caminho) : Caminho(std::filesystem::path(caminho).string())
{
    carregarModelo(std::filesystem::path(caminho).string());
}

void Arquivo3d::exibirInformacoes() 
{
     Debug::emitir("IMPORTADOR", "Extensão do arquivo: " + obterExtensaoArquivo(Caminho));
     Debug::emitir("IMPORTADOR", "Nome da cena: " + std::string(cena->mName.C_Str()));
     Debug::emitir("IMPORTADOR", ">> Número de malhas: " + std::to_string(cena->mNumMeshes));
     for (size_t i = 0; i < vertices.size(); ++i) {
         Debug::emitir("IMPORTADOR", ">> Malha " + std::to_string(i));
         Debug::emitir("IMPORTADOR", "    Número de vértices: " + std::to_string(vertices[i].vertices.size() / 3));
         Debug::emitir("IMPORTADOR", "    Número de índices: " + std::to_string(vertices[i].indices.size()));
         Debug::emitir("IMPORTADOR", "    Número de coordenadas UV: " + std::to_string(vertices[i].uvs.size() / 2));
         Debug::emitir("IMPORTADOR", "    Número de normais: " + std::to_string(vertices[i].normals.size() / 3));
     }
}
void Arquivo3d::carregarModelo(const std::string& caminho) {
    
    for (const auto& arquivo : arquivos)
    {
        if (arquivo.second == caminho) {
            Debug::emitir(Debug::Alerta, "Arquivo 3d já existente, re-utilizando");
            vertices = arquivo.first.first;
            materiais = arquivo.first.second;
            Caminho = arquivo.second;
            return;
        }

    }
    
    Assimp::Importer importador;
    cena = importador.ReadFile(caminho, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipUVs);

    if (!cena || cena->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !cena->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importador.GetErrorString() << std::endl;
        return;
    }

    extrairDados();
    arquivos.push_back(std::make_pair(std::make_pair(vertices, materiais), caminho));
}
void Arquivo3d::extrairDados() {
    if (!cena) return;

    vertices.resize(cena->mNumMeshes);

    for (unsigned int i = 0; i < cena->mNumMeshes; ++i) {
        aiMesh* mesh = cena->mMeshes[i];
        if (!mesh) continue;

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
void Arquivo3d::extrairMateriais() {
    if (!cena) return;
    for (unsigned int i = 0; i < cena->mNumMaterials; ++i) {
        aiMaterial* material = cena->mMaterials[i];
        Material* mat = new Material();

        aiColor3D cor;

        aiString path;
        // extrair textura difusa
        if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &path)) {
           mat->textura_difusa->path = path.C_Str(); // Armazena a textura
           Debug::emitir("Textura Difusa", material->GetName().C_Str() + std::string(": ") + path.C_Str());
        }
        // extrair cor difusa
        if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, cor)) {
            mat->difusa = { cor.r, cor.g, cor.b };
        }

        materiais.push_back(mat);
    }
}
std::string Bubble::Arquivadores::Arquivo3d::nome() const
{
    return cena->mName.C_Str();
}
std::string Arquivo3d::obterExtensaoArquivo(std::string& caminho) {
    size_t posPonto = caminho.find_last_of('.');
    if (posPonto == std::string::npos) {
        return "";
    }
    return caminho.substr(posPonto + 1);
}