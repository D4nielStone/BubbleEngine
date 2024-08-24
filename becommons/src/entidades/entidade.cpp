#include "becommons.hpp"
#include "entidade.hpp"
#include "rapidjson/stringbuffer.h"
#include "src/cena/scenemanager.hpp"

using namespace Bubble::Entidades;

Entidade::~Entidade() = default;

Entidade::Entidade(const char* name)
    : ativado(true), Nome(name), transformacao(std::make_shared<Bubble::Componentes::Transformacao>()), Componentes({ transformacao }) {}

Entidade::Entidade(Bubble::Arquivadores::Arquivo3d arquivo_objeto)
    : transformacao(std::make_shared<Bubble::Componentes::Transformacao>()), Componentes({ transformacao }) {
    carregarModelo(std::move(arquivo_objeto));
}

Entidade::Entidade()
    : transformacao(std::make_shared<Bubble::Componentes::Transformacao>()), Componentes({ transformacao }) {}

void Entidade::atualizar(float deltaTime) {
    if (!ativado) return;

    for (const auto& componente : Componentes) {
        if (!dynamic_cast<Componentes::Camera*>(componente.get()) && !dynamic_cast<Componentes::Renderizador*>(componente.get())) {
            componente->atualizar(deltaTime);
        }
    }
}

void Entidade::renderizar() {
    if (!ativado) return;

    for (const auto& componente : Componentes) {
        if (auto renderizador = dynamic_cast<Componentes::Renderizador*>(componente.get())) {
            obterTransformacao()->atualizar();
            renderizador->atualizar(0);
        }
    }
}

std::string* Entidade::nome() {
    return &Nome;
}

void Entidade::carregarModelo(Bubble::Arquivadores::Arquivo3d object_file) {
    auto renderizador = std::make_shared<Bubble::Componentes::Renderizador>(std::move(object_file));
    adicionarComponente(renderizador);
    Nome = renderizador->nome();
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

std::shared_ptr<Bubble::Componentes::Transformacao> Entidade::obterTransformacao() {
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
