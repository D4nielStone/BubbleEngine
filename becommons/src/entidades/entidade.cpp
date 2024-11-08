// Copyright (c) 2024 Daniel Oliveira
// Licenciado sob a licença MIT. Consulte o arquivo LICENSE para mais informaçoes.
#include "glad/glad.h"
#include "entidade.hpp"
#include "rapidjson/stringbuffer.h"
#include "src/cena/scenemanager.hpp"

using namespace Bubble::Entidades;

Entidade::~Entidade() = default;

Entidade::Entidade(const char* name)
    : ativado(true), Nome(std::make_shared<std::string>(name)), transformacao(std::make_shared<Bubble::Componentes::Transformacao>()), Componentes({ transformacao }) {}

Entidade::Entidade(const Arquivadores::Arquivo3d& arquivo_objeto)
    : transformacao(std::make_shared<Bubble::Componentes::Transformacao>()), Componentes({ transformacao }) {
    carregarNode(arquivo_objeto.RootNode);
}

Entidade::Entidade()
    : transformacao(std::make_shared<Bubble::Componentes::Transformacao>()), Componentes({ transformacao }) {}

void Entidade::atualizar() const {
    
    if (!ativado || !ativado_root)
    {
        for (auto& filho : filhos)
            filho->ativado_root = false;
        return;
    }
    else
    {
        for (auto& filho : filhos)
            filho->ativado_root = true;
    }

    for (const auto& componente : Componentes) 
    {
        if (componente->nome() != "Renderizador" &&componente->nome() != "Terreno" && componente->nome() != "Camera"&& componente->nome() != "Transformacao") {
            componente->atualizar();
        }
    }
}
// Método para renderizar Entidade
void Entidade::renderizar()
{
    if (!ativado || !ativado_root) return;

    transformacao->atualizar();
    for (const auto& componente : Componentes)
    {
        if (componente->nome() == "Renderizador" || componente->nome() == "Terreno")
            componente->atualizar();
    }

}

std::string Entidade::nome() const
{
    return *Nome.get();
}

std::shared_ptr<std::string> Bubble::Entidades::Entidade::nomeptr()
{
    return Nome;
}

void Entidade::carregarNode(const Node& node)
{
    *Nome = node.nome;
    //if (pai)
    //{
    //    transformacao->definirMatriz(node.transformacao * painel->obterTransformacao()->obterMatriz());
    //}
    //else
    //{
    //transformacao->definirMatriz(node.transformacao);
    //}
    for (const auto& malha : node.malhas)
    {
        adicionarComponente(std::make_shared<Bubble::Componentes::Renderizador>(malha));
    }
    
    for (const auto& no_filho : node.filhos)
    {
        auto entidade_filho = std::make_shared<Entidade>();
        entidade_filho->pai = this;
        entidade_filho->carregarNode(no_filho);
        filhos.push_back(entidade_filho);
    }
}

std::shared_ptr<Bubble::Comum::Componente> Entidade::obterComponente(const std::string& nome) {
    auto it = std::find_if(Componentes.begin(), Componentes.end(),
        [&nome](const std::shared_ptr<Bubble::Comum::Componente>& componente) { return componente->nome() == nome; });
    return (it != Componentes.end()) ? *it : nullptr;
}

std::vector<std::shared_ptr<Bubble::Comum::Componente>> Entidade::obterComponentes(const std::string& nome)
const {
    std::vector<std::shared_ptr<Bubble::Comum::Componente>> comps;
    for (const auto& componente : Componentes) {
        if (componente->nome() == nome) {
            comps.push_back(componente);
        }
    }
    return comps;
}

const std::vector<std::shared_ptr<Entidade>>& Bubble::Entidades::Entidade::obterFilhos() const
{
    return filhos;
}

std::shared_ptr<Bubble::Componentes::Transformacao> Entidade::obterTransformacao() const 
{
    return transformacao;
}

const std::vector<std::shared_ptr<Bubble::Comum::Componente>>& Entidade::listaDeComponentes() const {
    return Componentes;
}

bool Entidade::adicionarComponente(std::shared_ptr<Bubble::Comum::Componente> componente) {
    componente->definirPai(this);
    Componentes.push_back(componente);
    return true;
}

rapidjson::Value Entidade::serializar(rapidjson::Document* v) {
    rapidjson::Value obj(rapidjson::kObjectType);
    obj.AddMember("nome", rapidjson::Value().SetString(Nome->c_str(), v->GetAllocator()), v->GetAllocator());

    rapidjson::Value array(rapidjson::kArrayType);
    array.PushBack(transformacao->serializar(v), v->GetAllocator());

    obj.AddMember("transformacao", array, v->GetAllocator());
    return obj;
}

bool Entidade::parse(rapidjson::Value& entidade) {
    if (!entidade.HasMember("nome")) {
        Debug::emitir("ENTIDADE", "  Erro: Entidade sem nome.");
        return false;
    }
    *Nome = entidade["nome"].GetString();
    Debug::emitir("ENTIDADE", "  " + *Nome.get() + ":");
    return true;
}
