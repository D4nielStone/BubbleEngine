
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "transformacao.hpp"
#include <src/arquivadores/shader.cpp>
#include "src/cena/scene.hpp"
using namespace Bubble::Componentes;

Transformacao::Transformacao()
    : matriz_de_modelo(glm::mat4(1.f)), posicao(0.f, 0.f, 0.f), rotacao(0, 0, 0), escala(1, 1, 1)
{
    Nome = "Transformacao";
    variaveis.push_back(std::pair(& posicao, "Posicao"));
    variaveis.push_back(std::pair(& rotacao, "Rotacao"));
    variaveis.push_back(std::pair(& escala, "Escala"));
}

rapidjson::Value Transformacao::serializar(rapidjson::Document* doc) const 
{
    rapidjson::Value obj(rapidjson::kObjectType);
    rapidjson::Value positionarr(rapidjson::kArrayType);
    positionarr.PushBack(rapidjson::Value().SetFloat(posicao.x), doc->GetAllocator());
    positionarr.PushBack(rapidjson::Value().SetFloat(posicao.y), doc->GetAllocator());
    positionarr.PushBack(rapidjson::Value().SetFloat(posicao.z), doc->GetAllocator());
    obj.AddMember("posicao", positionarr, doc->GetAllocator());
    rapidjson::Value rotacaoarr(rapidjson::kArrayType);
    rotacaoarr.PushBack(rapidjson::Value().SetFloat(rotacao.x), doc->GetAllocator());
    rotacaoarr.PushBack(rapidjson::Value().SetFloat(rotacao.y), doc->GetAllocator());
    rotacaoarr.PushBack(rapidjson::Value().SetFloat(rotacao.z), doc->GetAllocator());
    obj.AddMember("rotacao", rotacaoarr, doc->GetAllocator());
    rapidjson::Value escalaarr(rapidjson::kArrayType);
    escalaarr.PushBack(rapidjson::Value().SetFloat(escala.x), doc->GetAllocator());
    escalaarr.PushBack(rapidjson::Value().SetFloat(escala.y), doc->GetAllocator());
    escalaarr.PushBack(rapidjson::Value().SetFloat(escala.z), doc->GetAllocator());
    obj.AddMember("escala", escalaarr, doc->GetAllocator());
    return obj;
}
bool Transformacao::parse(rapidjson::Value& transform)
{
    // Verificar se "posicao" é um array de 3 elementos
    if (!transform.HasMember("posicao") || !transform["posicao"].IsArray() || transform["posicao"].Size() != 3)
        return false;

    // Verificar se "rotacao" é um array de 3 elementos
    if (!transform.HasMember("rotacao") || !transform["rotacao"].IsArray() || transform["rotacao"].Size() != 3)
        return false;

    // Verificar se "escala" é um array de 3 elementos
    if (!transform.HasMember("escala") || !transform["escala"].IsArray() || transform["escala"].Size() != 3)
        return false;

    // Extrair valores para 'posicao'
    posicao.x = transform["posicao"][0].GetFloat();
    posicao.y = transform["posicao"][1].GetFloat();
    posicao.z = transform["posicao"][2].GetFloat();

    // Extrair valores para 'rotacao'
    rotacao.x = transform["rotacao"][0].GetFloat();
    rotacao.y = transform["rotacao"][1].GetFloat();
    rotacao.z = transform["rotacao"][2].GetFloat();

    // Extrair valores para 'escala'
    escala.x = transform["escala"][0].GetFloat();
    escala.y = transform["escala"][1].GetFloat();
    escala.z = transform["escala"][2].GetFloat();
    comporMatriz(glm::vec3(posicao.x, posicao.y, posicao.z), glm::vec3(rotacao.x, rotacao.y, rotacao.z), glm::vec3(escala.x, escala.y, escala.z));
    return true;
}

glm::vec3 Transformacao::obterRotacao() const { return glm::vec3(rotacao.x, rotacao.y, rotacao.z); }
Vector3<float> Transformacao::obterPosicao() const { return posicao; }
Vector3<float> Transformacao::obterEscala() const { return escala; }
Vector3<float> Transformacao::obterDirecao() const {
    const auto direcaoPadrao = Vector3<float>{ 0.0f, 0.0f, -1.0f };
    return rotacao * direcaoPadrao;
}

float* Transformacao::obterMatrizGlobal() const {
    return (float*)glm::value_ptr(matriz_de_modelo);
}
glm::mat4 Transformacao::obterMatriz() const
{
    return matriz_de_modelo;
}

void Transformacao::atualizar() {
    if (!shader_atual) return;

    comporMatriz({ posicao.x,posicao.y,posicao.z }, { rotacao.x,rotacao.y,rotacao.z }, { escala.x,escala.y,escala.z });
    shader_atual->setMat4("model", glm::value_ptr(matriz_de_modelo));
}
void Transformacao::configurar() {
    glm::translate(matriz_de_modelo, glm::vec3(posicao.x, posicao.y, posicao.z));
    glm::rotate(matriz_de_modelo, 1.f, glm::vec3(rotacao.x, rotacao.y, rotacao.z));
    glm::scale(matriz_de_modelo, glm::vec3(escala.x, escala.y, escala.z));
}

void Transformacao::definirPosicao(const Vector3<float>& newPosition) { posicao = newPosition;  comporMatriz(glm::vec3(posicao.x,posicao.y,posicao.z), glm::vec3(rotacao.x, rotacao.y, rotacao.z), glm::vec3(escala.x, escala.y, escala.z));}
void Transformacao::definirRotacao(const Vector3<float>& newRotation) { rotacao = newRotation; comporMatriz(glm::vec3(posicao.x, posicao.y, posicao.z), glm::vec3(rotacao.x, rotacao.y, rotacao.z), glm::vec3(escala.x, escala.y, escala.z));
}
void Transformacao::definirEscala(const Vector3<float>& newScale) { escala = newScale;  comporMatriz(glm::vec3(posicao.x, posicao.y, posicao.z), glm::vec3(rotacao.x, rotacao.y, rotacao.z), glm::vec3(escala.x, escala.y, escala.z));
}

void Transformacao::Move(Vector3<float> &pos) {
    posicao += pos;
    glm::translate(matriz_de_modelo, glm::vec3(posicao.x,posicao.y,posicao.z));
}
void Transformacao::Move(glm::vec3 pos) {
    posicao += Vector3<float>(pos.x,pos.y,pos.z);
    glm::translate(matriz_de_modelo, glm::vec3(posicao.x,posicao.y,posicao.z));
}
void Transformacao::Rotacionar(const float x, const float y, const float z) {
    glm::vec3 n = glm::normalize(glm::vec3(x, y, z));
    rotacao = Vector3(n.x,n.y,n.z);
    glm::rotate(matriz_de_modelo, 1.f, glm::vec3(rotacao.x,rotacao.y,rotacao.z));
}

void Transformacao::decomporMatriz(glm::vec3* position, glm::vec3* rotation, glm::vec3* scale) const {
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::quat orientation;
    glm::decompose(matriz_de_modelo, *scale, orientation, *position, skew, perspective);
    *rotation = glm::eulerAngles(orientation);
    *rotation = glm::degrees(*rotation);
}
void Transformacao::comporMatriz(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) {
    matriz_de_modelo = glm::translate(glm::mat4(1.0f), position);
    matriz_de_modelo = glm::rotate(matriz_de_modelo, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    matriz_de_modelo = glm::rotate(matriz_de_modelo, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    matriz_de_modelo = glm::rotate(matriz_de_modelo, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    matriz_de_modelo = glm::scale(matriz_de_modelo, scale);
}
void Transformacao::definirMatriz(glm::mat4 matriz_nova, int factor)
{
    glm::vec3 p, r, s;
    matriz_de_modelo = matriz_nova;
    decomporMatriz(&p, &r, &s);
    posicao = p / glm::vec3(factor); ///< 1 metro = 100 centímetros
    rotacao = r;
    escala = s / glm::vec3(factor);  ///< 1 metro = 100 centímetros
}
