#include "transformacao.hpp"

using namespace Bubble::Componentes;

Transformacao::Transformacao()
    : matriz_de_modelo(glm::mat4(1.f)), posicao(0.f, 0.f, 0.f), rotacao(0, 0, 0), escala(0.1f)
{
    Nome = "Transformacao";
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
glm::quat Transformacao::obterRotacao() const { return rotacao; }
glm::vec3 Transformacao::obterPosicao() const { return posicao; }
glm::vec3 Transformacao::obterEscala() const { return escala; }
glm::vec3 Transformacao::obterDirecao() const {
    glm::vec3 direcaoPadrao = glm::vec3(0.0f, 0.0f, -1.0f);
    return rotacao * direcaoPadrao;
}
float* Transformacao::obterMatrizGlobal() const {
    return (float*)glm::value_ptr(matriz_de_modelo);
}
glm::mat4 Bubble::Componentes::Transformacao::obterMatriz() const
{
    return matriz_de_modelo;
}
void Transformacao::atualizar() {
    if (estado != DINAMICO)
        return;

    shader.use();
    shader.setMat4("model", glm::value_ptr(matriz_de_modelo));
}
void Transformacao::configurar() {
    glm::translate(matriz_de_modelo, posicao);
    glm::rotate(matriz_de_modelo, 1.f, rotacao);
    glm::scale(matriz_de_modelo, escala);
}
void Transformacao::definirPosicao(const glm::vec3& newPosition) { posicao = newPosition;  comporMatriz(posicao, rotacao, escala);}
void Transformacao::definirRotacao(const glm::vec3& newRotation) { rotacao = newRotation; comporMatriz(posicao, rotacao, escala); }
void Transformacao::definirEscala(const glm::vec3& newScale) { escala = newScale;  comporMatriz(posicao, rotacao, escala); }
void Transformacao::Move(glm::vec3 pos) {
    posicao += pos;
    glm::translate(matriz_de_modelo, posicao);
}
void Transformacao::Rotacionar(const float x, const float y, const float z) {
    rotacao = glm::normalize(glm::vec3(x, y, z));
    glm::rotate(matriz_de_modelo, 1.f, rotacao);
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

void Bubble::Componentes::Transformacao::definirMatriz(glm::mat4 matriz_nova)
{
    matriz_de_modelo = matriz_nova;
}
