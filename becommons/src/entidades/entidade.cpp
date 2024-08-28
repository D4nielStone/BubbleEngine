#include "entidade.hpp"
#include "rapidjson/stringbuffer.h"
#include "src/cena/scenemanager.hpp"

using namespace Bubble::Entidades;

Entidade::~Entidade() = default;

Entidade::Entidade(const char* name)
    : ativado(true), Nome(name), transformacao(std::make_shared<Bubble::Componentes::Transformacao>()), Componentes({ transformacao }) {}

Entidade::Entidade(const Arquivadores::Arquivo3d& arquivo_objeto)
    : transformacao(std::make_shared<Bubble::Componentes::Transformacao>()), Componentes({ transformacao }) {
    carregarNode(arquivo_objeto.RootNode);
}

Entidade::Entidade()
    : transformacao(std::make_shared<Bubble::Componentes::Transformacao>()), Componentes({ transformacao }) {}

void Entidade::atualizar(float deltaTime) const {
    if (!ativado) return;

    for (const auto& componente : Componentes) {
        if (componente->nome() != "Renderizador" && componente->nome() != "Camera"&& componente->nome() != "Transformacao") {
            componente->atualizar(deltaTime);
        }
    }
}

void Entidade::renderizar() const {
    if (!ativado) return;

    for (const auto& componente : Componentes) {
        if (componente->nome() == "Renderizador")
        {
            transformacao->atualizar();
            componente->atualizar(0);
        }
    }
}

std::string Entidade::nome() const
{
    return Nome;
}

void Entidade::carregarNode(const Node& node)
{
    Nome = node.nome;
    if (pai)
    {
        transformacao->definirMatriz(node.transformacao * pai->obterTransformacao()->obterMatriz());
    }
    else
    {
    transformacao->definirMatriz(node.transformacao);
    }
    if (node.malhas.size() > 0)
    {
        for (auto& malha : node.malhas)
        {
            auto renderizador = std::make_shared<Bubble::Componentes::Renderizador>(malha);
            adicionarComponente(renderizador);
        }
    }
    for (auto& no_filho : node.filhos)
    {
        auto entidade_filho = std::make_shared<Entidade>();
        entidade_filho->pai = this;
        entidade_filho->carregarNode(no_filho);
        filhos.emplace_back(std::move(entidade_filho));
    }
}

std::shared_ptr<Bubble::Comum::Componente> Entidade::obterComponente(const std::string& nome) {
    auto it = std::find_if(Componentes.begin(), Componentes.end(),
        [&nome](const std::shared_ptr<Bubble::Comum::Componente>& componente) { return componente->nome() == nome; });
    return (it != Componentes.end()) ? *it : nullptr;
}

std::unordered_set<std::shared_ptr<Bubble::Comum::Componente>> Entidade::obterComponentes(const std::string& nome) {
    std::unordered_set<std::shared_ptr<Bubble::Comum::Componente>> comps;
    for (const auto& componente : Componentes) {
        if (componente->nome() == nome) {
            comps.insert(componente);
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

const std::unordered_set<std::shared_ptr<Bubble::Comum::Componente>>& Entidade::listaDeComponentes() const {
    return Componentes;
}

void Entidade::adicionarComponente(std::shared_ptr<Bubble::Comum::Componente> componente) {
    componente->definirPai(this);
    Componentes.insert(std::move(componente));
}

rapidjson::Value Entidade::serializar(rapidjson::Document* v) {
    rapidjson::Value obj(rapidjson::kObjectType);
    obj.AddMember("nome", rapidjson::Value().SetString(Nome.c_str(), v->GetAllocator()), v->GetAllocator());

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
    Nome = entidade["nome"].GetString();
    Debug::emitir("ENTIDADE", "  " + Nome + ":");
    return true;
}