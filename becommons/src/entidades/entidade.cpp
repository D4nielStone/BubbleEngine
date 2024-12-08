/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "glad/glad.h"
#include "entidade.hpp"
#include "rapidjson/stringbuffer.h"
#include "src/cena/scenemanager.hpp"

using namespace Bubble::Entidades;

Entidade::~Entidade() {
    liberar();
}

Entidade::Entidade(const char* name)
    : ativado(true), Nome(std::make_shared<std::string>(name)), transformacao(std::make_shared<Bubble::Componentes::Transformacao>()), Componentes({ transformacao }) {}

Entidade::Entidade(const Arquivadores::Arquivo3d& arquivo_objeto)
    : transformacao(std::make_shared<Bubble::Componentes::Transformacao>()), Componentes({ transformacao }) {
    if(arquivo_objeto.carregado())
    carregarNode(arquivo_objeto.RootNode);
}

Entidade::Entidade()
    : transformacao(std::make_shared<Bubble::Componentes::Transformacao>()), Componentes({ transformacao }) {}

void Entidade::atualizar() const {
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
    for (const auto& filho : filhos)
    {
        filho->renderizar();
    }
}

std::string Entidade::nome() const
{
    return *Nome.get();
}

void Bubble::Entidades::Entidade::liberar()
{
    if (obterComponente("Camera") && static_cast<Componentes::Camera*>(obterComponente("Camera").get()) == Bubble::Cena::obterSceneManager()->cenaAtual()->camera_principal)
        Bubble::Cena::obterSceneManager()->cenaAtual()->camera_principal->liberar();
    for (auto& f : filhos)
    {
        f->liberar();
    }
    filhos.clear();
    for (auto& f : Componentes)
    {
        f->liberar();
    }
    Componentes.clear();
}

std::shared_ptr<std::string> Bubble::Entidades::Entidade::nomeptr()
{
    return Nome;
}

void Bubble::Entidades::Entidade::removerFilho(std::shared_ptr<Entidade> ent)
{
    auto it = std::find(filhos.begin(), filhos.end(), ent);
    filhos.erase(it);
}

void Entidade::carregarNode(const Node& node)
{
    *Nome = node.nome;
    
    transformacao->definirMatriz(node.transformacao, node.metrica);
    for (const auto& malha : node.malhas)
    {
        adicionarComponente(std::make_shared<Bubble::Componentes::Renderizador>(malha));
    }
    if (node.camera) adicionarComponente(node.camera);
    
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
    if (Bubble::Cena::obterSceneManager()->cenaAtual())
        Bubble::Cena::obterSceneManager()->cenaAtual()->renderizadores.adicionarEntidade(shared_from_this());
    return true;
}

rapidjson::Value Entidade::serializar(rapidjson::Document* v) {
    rapidjson::Value obj(rapidjson::kObjectType);
    /// Adiciona nome
    obj.AddMember("nome", rapidjson::Value().SetString(Nome->c_str(), v->GetAllocator()), v->GetAllocator());

    /// array filhos
    rapidjson::Value arr(rapidjson::kArrayType);

    for (const auto& filho : filhos)
    {
        arr.PushBack(filho->serializar(v).GetObject(), v->GetAllocator());
    }
    obj.AddMember("filhos", arr, v->GetAllocator());

    /// Adiciona transform

    if (obterComponente("Transformacao"))
    {
        rapidjson::Value array(rapidjson::kArrayType);
        array.PushBack(transformacao->serializar(v), v->GetAllocator());

        obj.AddMember("transformacao", array, v->GetAllocator());
    }
    /// Adiciona renderizador

    if (obterComponente("Renderizador"))
    {
        rapidjson::Value array(rapidjson::kArrayType);
        array.PushBack(obterComponente("Renderizador")->serializar(v), v->GetAllocator());

        obj.AddMember("renderizador", array, v->GetAllocator());
    }
    /// Adiciona camera

    if (obterComponente("Camera"))
    {
        rapidjson::Value array(rapidjson::kArrayType);
        array.PushBack(obterComponente("Camera")->serializar(v), v->GetAllocator());

        obj.AddMember("camera", array, v->GetAllocator());
    }
    return obj;
}

bool Entidade::parse(rapidjson::Value& entidade) 
{
    if (!entidade.HasMember("nome")) {
        Debug::emitir("Entidade", "  Erro: Entidade sem nome.");
        return false;
    }
    if (!entidade.HasMember("transformacao")) {
        Debug::emitir("Entidade", "  Erro: Entidade sem transformacao.");
        return false;
    }
    /// Atribui nome
    *Nome = entidade["nome"].GetString();
    /// Atribui transform

    for (auto& tr : entidade["transformacao"].GetArray()) {
        if (!transformacao->parse(tr)) {
            Debug::emitir("Entidade", "  Erro ao parsear transformacao.");
            return false;
        }
    }

    if (entidade.HasMember("renderizador")) 
    {
        /// Atribui renderizador
        for (auto& rd : entidade["renderizador"].GetArray()) {
        auto render = std::make_shared<Componentes::Renderizador>();
            if (!render->parse(rd)) {
                Debug::emitir("Entidade", "  Erro ao parsear renderizador.");
            }
            else
            {
                adicionarComponente(render);
                render->configurar();
            }
        }
    }
    if (entidade.HasMember("camera")) 
    {
        /// Atribui renderizador
        for (auto& rd : entidade["camera"].GetArray()) {
        auto cam = std::make_shared<Componentes::Camera>();
            if (!cam->parse(rd)) {
                Debug::emitir("Entidade", "  Erro ao parsear camera.");
            }
            else
            {
                adicionarComponente(cam);
                cam->configurar();
            }
        }
    }

    if (entidade.HasMember("filhos")) {
        for (auto& filho : entidade["filhos"].GetArray()) {
            auto filho_shared = std::make_shared<Entidade>();
            if (!filho_shared->parse(filho.GetObject())) {
                Debug::emitir("Entidade", "  Erro ao parsear filho.");
                return false;
            }
            filhos.push_back(filho_shared);
        }
    }

    return true;
}
