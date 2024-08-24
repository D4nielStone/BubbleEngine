#include "Scene.hpp"
#include "glad/glad.h"

namespace Bubble::Cena
{
    // Uma cena é criada
    // \param name: para o nome da cena
    Scene::Scene(const char* name) : Name(name) {
        Debug::emitir("CENA", std::string(name) + " criada");
    }
    Scene::~Scene() {}
    // Deve adicionar entidade
    void Scene::adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject) {
        if (!existeEntidade(gameObject.get())) {
            for (auto& c : gameObject->listaDeComponentes()) {
                if (!c->carregado())
                    c->configurar();
            }
            Entidades.insert(std::move(gameObject));
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
        for (auto& obj : Entidades) {
            obj->renderizar();
        }
    }
    // Deve atualizar Cena
    void Scene::atualizar(float deltaTime)
    {
        camera_editor.atualizar(deltaTime);
        for (auto& obj : Entidades) {
            obj->atualizar(deltaTime);
            if (obj->obterComponente("Camera"))
            {
                camera_principal = dynamic_cast<Bubble::Componentes::Camera*>(obj->obterComponente("Camera").get());
            }
        }
    }
    // Deve carregar Cena
    void Scene::carregar() {
        camera_editor.configurar();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        for(auto& entidade : Entidades)
        for (auto& c : entidade->listaDeComponentes()) {
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
            if (obj.get() == entidade) {
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