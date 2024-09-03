#include "Scene.hpp"
#include "glad/glad.h"

namespace Bubble::Cena
{
    // Uma cena é criada
    // \param name: para o nome da cena
    Scene::Scene(const char* name) : Name(name), skybox(std::make_unique<Util::Skybox>()) {
        Debug::emitir("CENA", std::string(name) + " criada");
    }
    Scene::~Scene() {}
    // Deve adicionar entidade
    void Scene::carregarComponentes(std::shared_ptr<Entidades::Entidade> entidade) {
        for (auto& componente : entidade->listaDeComponentes()) {
            if (!componente->carregado())
                componente->configurar();
        }
        for (auto& filho : entidade->obterFilhos()) {
            carregarComponentes(filho);
        }
    }

    void Scene::criarEntidade(std::unique_ptr<Arquivadores::Arquivo3d> arquivo_3d, const char* nome_entidade)
    {
        adicionarEntidade(std::make_shared<Entidades::Entidade>(*arquivo_3d));
    }

    void Scene::adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject) {
        if (!existeEntidade(gameObject.get())) {
            carregarComponentes(gameObject);  // Carrega os componentes da entidade e dos filhos
            Entidades.push_back(std::move(gameObject));  // Adiciona objeto à lista de entidades
        }
    }

    // Deve desenhar céu ( skybox, clear color ... )
    void Scene::desenharCeu()
    {
        glClearColor(1, 1, 1, 1);
    }
    // Deve renderizar Cena
    void Scene::renderizar(float aspecto) {
        camera_editor.atualizarAspecto(aspecto);
        skybox->renderizar();
        for (auto& obj : Entidades) {
            obj->renderizar();
            renderizarFilhos(obj, aspecto);
        }
    }
    void Scene::renderizarFilhos(std::shared_ptr<Entidades::Entidade> entidade, float aspecto)
    {
        for (auto& filho : entidade->obterFilhos()) {
            filho->renderizar();
            renderizarFilhos(filho, aspecto);
        }
    }
    // Deve atualizar Cena
    void Scene::atualizar() {
        camera_editor.atualizar();
        for (auto& obj : Entidades) {
            obj->atualizar();
            if (obj->selecionada)entidade_selecionada = obj;
            if (obj->obterComponente("Camera")) {
                camera_principal = static_cast<Componentes::Camera*>(obj->obterComponente("Camera").get());
            }
            atualizarFilhos(obj);
        }
    }

    void Scene::atualizarFilhos(std::shared_ptr<Entidades::Entidade> entidade) {
        for (auto& filho : entidade->obterFilhos()) {
            if (filho->selecionada)entidade_selecionada = filho;
            filho->atualizar();
            if (filho->obterComponente("Camera")) {
                camera_principal = static_cast<Componentes::Camera*>(filho->obterComponente("Camera").get());
            }
            atualizarFilhos(filho);
        }
    }

    // Deve carregar Cena
    void Scene::carregar() {
        camera_editor.configurar();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        for(auto& entidade : Entidades)
        for (auto& c : entidade->listaDeComponentes()) 
        {
            c->configurar();
        }
        //skybox.configurarBuffers();
    }
    // Deve serializar ela mesma ( isso é, passar para o documento json seus dados )
    void Scene::serializar(rapidjson::Document* doc) const {
        doc->SetObject();
        doc->AddMember("nome", rapidjson::Value().SetString(Name, doc->GetAllocator()), doc->GetAllocator());

        rapidjson::Value arr(rapidjson::kArrayType);
        for (auto& entidade : Entidades) {
            arr.PushBack(entidade->serializar(doc), doc->GetAllocator());
        }

        doc->AddMember("entidades", arr, doc->GetAllocator());
    }
    // Deve ler o arquivo json e obter os dados
    bool Scene::parse(rapidjson::Document& document)
    {
        // Deve verificar se o documento tem o membro "nome" e se é uma string
        if (document.HasMember("nome") && document["nome"].IsString())
        {
            Name = document["nome"].GetString();
            Debug::emitir("CENA", Name + std::string(":"));
        }
        else
        {
            Debug::emitir(Debug::Erro, "O membro 'nome' não foi encontrado na cena");
            return false;
        }
        // Deve analisar entidades
        if (document.HasMember("entidades") && document["entidades"].IsArray())
        {
            rapidjson::Value& entidades = document["entidades"];
            for (rapidjson::SizeType i = 0; i < entidades.Size(); ++i)
            {
                std::shared_ptr<Entidades::Entidade> entidadetmp = std::make_shared<Entidades::Entidade>();
                if (!entidadetmp->parse(entidades[i]))
                {
                    Debug::emitir(Debug::Erro, "parse de entidades");
                    return false;
                }
                else
                {
                    adicionarEntidade(entidadetmp);
                }
            }
        }
        else
        {
            Debug::emitir(Debug::Erro, "parse de entidades");
            return false;
        }
        return true;
    }
    // Deve retornar se a entidade existe na lista de entidades
    bool Scene::existeEntidade(Entidades::Entidade* entidade) const {
        for (const auto& obj : Entidades) {
            if (entidadeExisteRecursivo(obj, entidade)) {
                return true;
            }
        }
        return false;
    }

    bool Scene::entidadeExisteRecursivo(std::shared_ptr<Entidades::Entidade> obj, Entidades::Entidade* entidade) const {
        if (obj.get() == entidade) {
            return true;
        }
        for (const auto& filho : obj->obterFilhos()) {
            if (entidadeExisteRecursivo(filho, entidade)) {
                return true;
            }
        }
        return false;
    }

    // Deve retornar nome
    std::string Scene::nome() const {
        return Name;
    }
}