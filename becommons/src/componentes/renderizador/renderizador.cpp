
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "renderizador.hpp"
#include "src/arquivadores/imageloader.hpp"
#include <glad/glad.h>
#include <algorithm>
#include <src/util/skybox.hpp>
#include "src/nucleo/engine.hpp"
#include <sstream>
#include <filesystem>

using namespace Bubble::Componentes;
using namespace std::filesystem;

void Renderizador::configurar()
{
    configurarBuffers();
    carregadov = true;
}
void Renderizador::atualizar()
{ 
    glBindVertexArray(malha.VAO);
    GLenum mode{};
    if (visualizarWireFrame)
        mode = GL_LINE_LOOP;
    else
        mode = GL_TRIANGLES;
    glDrawElements(mode, static_cast<GLsizei>(malha.indices.size()), GL_UNSIGNED_INT, 0);
}
void Renderizador::salvarMalha() const
{
    Debug::emitir("Renderizador", "Salvando malha em :" + std::string(path_malha));
    create_directories(path_malha);

    const std::string nome = path_malha + "\\" + std::to_string(malha.id);

    std::ofstream arquivo(nome + ".bmalha", std::ios::binary); // Abre como binário

    // Escreve o nome da malha
    uint32_t nameLength = malha.nome.size();
    arquivo.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));  // Escreve o comprimento do nome
    arquivo.write(malha.nome.c_str(), nameLength);  // Escreve o nome

    // Escreve vértices
    uint32_t numVertices = malha.vertices.size();
    arquivo.write(reinterpret_cast<const char*>(&numVertices), sizeof(numVertices));
    arquivo.write(reinterpret_cast<const char*>(malha.vertices.data()), numVertices * sizeof(float));

    // Escreve normais
    uint32_t numNormals = malha.normals.size();
    arquivo.write(reinterpret_cast<const char*>(&numNormals), sizeof(numNormals));
    if (numNormals > 0) {
        arquivo.write(reinterpret_cast<const char*>(malha.normals.data()), numNormals * sizeof(float));
    }

    // Escreve UVs
    uint32_t numUVs = malha.uvs.size();
    arquivo.write(reinterpret_cast<const char*>(&numUVs), sizeof(numUVs));
    if (numUVs > 0) {
        arquivo.write(reinterpret_cast<const char*>(malha.uvs.data()), numUVs * sizeof(float));
    }

    // Escreve índices
    uint32_t numIndices = malha.indices.size();
    arquivo.write(reinterpret_cast<const char*>(&numIndices), sizeof(numIndices));
    arquivo.write(reinterpret_cast<const char*>(malha.indices.data()), numIndices * sizeof(unsigned int));

    // Escreve material
    arquivo.write(reinterpret_cast<const char*>(&malha.material.nome), sizeof(malha.material.nome));  // Nome do material
    arquivo.write(reinterpret_cast<const char*>(&malha.material.difusa), sizeof(malha.material.difusa));  // Cor difusa
    arquivo.write(reinterpret_cast<const char*>(&malha.material.especular), sizeof(malha.material.especular));  // Cor especular

    // Escreve texturas
    uint32_t numTextures = malha.material.texturas.size();
    arquivo.write(reinterpret_cast<const char*>(&numTextures), sizeof(numTextures));
    for (const auto& tex : malha.material.texturas) {
        uint32_t texPathLength = tex.path.size();
        arquivo.write(reinterpret_cast<const char*>(&texPathLength), sizeof(texPathLength));  // Escreve o comprimento do caminho da textura
        arquivo.write(tex.path.c_str(), texPathLength);  // Escreve o caminho da textura

        // Escreve o tipo da textura
        uint32_t tipoLength = tex.tipo.size();
        arquivo.write(reinterpret_cast<const char*>(&tipoLength), sizeof(tipoLength));  // Escreve o comprimento do tipo
        arquivo.write(tex.tipo.c_str(), tipoLength);  // Escreve o tipo da textura
    }

    arquivo.close();
}
rapidjson::Value Renderizador::serializar(rapidjson::Document* doc) const
{
    rapidjson::Value obj(rapidjson::kObjectType); 
    // adiciona nome nome da malha
    rapidjson::Value nomeValue;
    nomeValue.SetString(malha.nome.c_str(), doc->GetAllocator());
    obj.AddMember("nome", nomeValue, doc->GetAllocator());

    // adiciona nome id da malha

    rapidjson::Value idValues;
    idValues.SetString(std::to_string(malha.id).c_str(), doc->GetAllocator());
    obj.AddMember("id", idValues, doc->GetAllocator());

    salvarMalha();

    return obj;
}
bool Bubble::Componentes::Renderizador::parse(rapidjson::Value& renderizador)
{
    // Verificar se "nome" é um string
    if (!renderizador.HasMember("nome") || !renderizador["nome"].IsString())
        return false;
    // Verificar se "id" é um int
    if (!renderizador.HasMember("id") || !renderizador["id"].IsString())
        return false;

    lerMalha(renderizador["nome"].GetString(),  renderizador["id"].GetString());
    return true;
}
void Renderizador::lerMalha(const std::string& malha_path, const std::string& malha_id)
{
    path_malha = Engine::getInstance()->diretorio_projeto + "\\Malhas\\" + malha_path;

    // Ler material e malha
    Debug::emitir("Renderizador", "Carregando malha de :" + path_malha + "\\" + malha_id);

    std::ifstream arquivo(path_malha +"\\" + malha_id + ".bmalha", std::ios::binary); // Abre como binário

    if (!arquivo.is_open()) {
        Debug::emitir("Renderizador", "Falha ao abrir o arquivo.");
        return;
    }

    // Lê o nome da malha
    uint32_t nameLength;
    arquivo.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    malha.nome.resize(nameLength);
    arquivo.read(&malha.nome[0], nameLength);

    // Lê os vértices
    uint32_t numVertices;
    arquivo.read(reinterpret_cast<char*>(&numVertices), sizeof(numVertices));
    malha.vertices.resize(numVertices);
    arquivo.read(reinterpret_cast<char*>(malha.vertices.data()), numVertices * sizeof(float));

    // Lê as normais
    uint32_t numNormals;
    arquivo.read(reinterpret_cast<char*>(&numNormals), sizeof(numNormals));
    malha.normals.resize(numNormals);
    if (numNormals > 0) {
        arquivo.read(reinterpret_cast<char*>(malha.normals.data()), numNormals * sizeof(float));
    }

    // Lê os UVs
    uint32_t numUVs;
    arquivo.read(reinterpret_cast<char*>(&numUVs), sizeof(numUVs));
    malha.uvs.resize(numUVs);
    if (numUVs > 0) {
        arquivo.read(reinterpret_cast<char*>(malha.uvs.data()), numUVs * sizeof(float));
    }

    // Lê os índices
    uint32_t numIndices;
    arquivo.read(reinterpret_cast<char*>(&numIndices), sizeof(numIndices));
    malha.indices.resize(numIndices);
    arquivo.read(reinterpret_cast<char*>(malha.indices.data()), numIndices * sizeof(unsigned int));

    // Lê o material
    arquivo.read(reinterpret_cast<char*>(&malha.material.nome), sizeof(malha.material.nome)); // Nome do material
    arquivo.read(reinterpret_cast<char*>(&malha.material.difusa), sizeof(malha.material.difusa)); // Cor difusa
    arquivo.read(reinterpret_cast<char*>(&malha.material.especular), sizeof(malha.material.especular)); // Cor especular

    // Lê as texturas
    uint32_t numTextures;
    arquivo.read(reinterpret_cast<char*>(&numTextures), sizeof(numTextures));
    malha.material.texturas.resize(numTextures);
    for (auto& tex : malha.material.texturas) {
        uint32_t texPathLength;
        arquivo.read(reinterpret_cast<char*>(&texPathLength), sizeof(texPathLength)); // Lê o comprimento do caminho da textura
        tex.path.resize(texPathLength);
        arquivo.read(&tex.path[0], texPathLength); // Lê o caminho da textura

        // Lê o tipo da textura (difusa, normal map, etc.)
        uint32_t tipoLength;
        arquivo.read(reinterpret_cast<char*>(&tipoLength), sizeof(tipoLength)); // Lê o comprimento do tipo
        tex.tipo.resize(tipoLength);
        arquivo.read(&tex.tipo[0], tipoLength); // Lê o tipo da textura
    }

    arquivo.close();

    Nome = "Renderizador";
    std::vector<std::any> arvore_material;
    arvore_material.push_back(std::pair(&this->malha.material.nome, "Nome do material"));
    arvore_material.push_back(std::pair(&this->malha.material.difusa, "Cor difusa"));
    arvore_material.push_back(std::pair(&visualizarWireFrame, "Modo de arame"));
    variaveis.push_back(std::pair(arvore_material, "Material"));
}
void Renderizador::configurarBuffers()
{
    if (!malha.carregado)
        for (auto& tex : malha.material.texturas)
        {
            tex.ID = Bubble::Arquivadores::TextureLoader::getInstance().carregarTextura(tex.path);
        }
    else
        return;
    //vertex buffer
    glGenVertexArrays(1, &malha.VAO);
    glGenBuffers(1, &malha.VBO);
    glGenBuffers(1, &malha.EBO);

    glBindVertexArray(malha.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, malha.VBO);

    size_t vertexSize = malha.vertices.size() * sizeof(float);
    size_t normalSize = malha.normals.size() * sizeof(float);
    size_t uvSize = malha.uvs.size() * sizeof(float);
    size_t totalSize = vertexSize + normalSize + uvSize;

    glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, malha.vertices.data());

    if (!malha.normals.empty()) {
        glBufferSubData(GL_ARRAY_BUFFER, vertexSize, normalSize, malha.normals.data());
    }

    if (!malha.uvs.empty()) {
        glBufferSubData(GL_ARRAY_BUFFER, vertexSize + normalSize, uvSize, malha.uvs.data());
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, malha.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, malha.indices.size() * sizeof(unsigned int), malha.indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    if (!malha.normals.empty()) {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vertexSize));
        glEnableVertexAttribArray(1);
    }

    if (!malha.uvs.empty()) {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vertexSize + normalSize));
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    malha.carregado = true;
}
Renderizador::Renderizador(const Malha& malha) : malha(malha)
{
    path_malha = Engine::getInstance()->diretorio_projeto + "\\Malhas\\" + malha.nome;
    Nome = "Renderizador";
    std::vector<std::any> arvore_material;
    arvore_material.push_back(std::pair(&this->malha.material.nome, "Nome do material"));
    arvore_material.push_back(std::pair(&this->malha.material.difusa, "Cor difusa"));
    arvore_material.push_back(std::pair(&visualizarWireFrame, "Modo de arame"));
    variaveis.push_back(std::pair(arvore_material, "Material"));
}
Renderizador::Renderizador(const std::string& malha_path, const std::string &malha_id)
{
    lerMalha(malha_path, malha_id);
    
    

}
Renderizador::~Renderizador()
{
    liberar();
}
void Renderizador::liberar()
{
    if (!carregadov) return;
    // Liberar texturas e buffers
    glDeleteVertexArrays(1, &malha.VAO);
    glDeleteBuffers(1, &malha.VBO);
    glDeleteBuffers(1, &malha.EBO);
    carregadov = false;
}
Malha& Bubble::Componentes::Renderizador::obterMalha()

{
    return malha;
}