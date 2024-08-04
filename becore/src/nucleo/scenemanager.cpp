#include "scenemanager.h"
#include "becore.h"
#include "src/depuracao/debug.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Bubble::Nucleo
{
    Scene::Scene(const char* name) : Name(name) {
        Debug::emitir("CENA", std::string(name) + " criada");
    }
    Scene::~Scene() {}
    SceneManager::SceneManager() : currentSceneIndex(-1)
    {
    }
    SceneManager::~SceneManager() {}
    void Scene::adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject) {
        if (!existeEntidade(gameObject.get())) {
            for (auto& c : gameObject->listaDeComponentes()) {
                c->configurar();
            }
            Entidades.push_back(gameObject);
        }
    }
    void Scene::desenharCeu()
    {
        glClearColor(1, 1, 1, 1);
    }
    void Scene::renderizar(float aspecto) {

        //desenharCeu();
        camera_editor.atualizarAspecto(aspecto);
        camera_editor.atualizar();
        for (auto& obj : Entidades) {
            obj->renderizar();
        }
    }
    void Scene::atualizar(float deltaTime)
    {
        for (auto& obj : Entidades) {
            obj->atualizar(deltaTime);
            if (obj->obterComponente("Camera"))
            {
                camera_principal = dynamic_cast<Bubble::Componentes::Camera*>(obj->obterComponente("Camera").get());
            }
        }
    }
    void Scene::carregar() {
        entidadeSelecionada = Entidades[Entidades.size() - 1].get();
        camera_editor.configurar();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        //skybox.configurarBuffers();

        for (auto& obj : Entidades) {
            for (auto& c : obj->listaDeComponentes()) {
                c->configurar();
            }
        }
    }
    void Scene::serializar(rapidjson::Document* doc) const {
        doc->SetObject();
        doc->AddMember("nome", rapidjson::Value().SetString(Name, doc->GetAllocator()), doc->GetAllocator());

        rapidjson::Value arr(rapidjson::kArrayType);
        for (auto& entidade : Entidades) {
            arr.PushBack(entidade->serializar(doc), doc->GetAllocator());
        }

        doc->AddMember("entidades", arr, doc->GetAllocator());
    }
    void SceneManager::adicionarCena(std::shared_ptr<Scene> scene) {
        scenes.push_back(scene);
    }
    void SceneManager::carregarCena(int sceneIndex) {
        if (sceneIndex >= 0 && sceneIndex < scenes.size()) {
            currentSceneIndex = sceneIndex;

            std::string msg("Carregando cena ");
            std::string soma = msg + std::to_string(currentSceneIndex);

            Debug::emitir(Debug::Tipo::Mensagem, soma.c_str());

            scenes[currentSceneIndex]->carregar();
        }
        else {
            Debug::emitir(Debug::Tipo::Erro, "índice da cena não é válido");
        }
    }
    void SceneManager::renderizarCenaAtual(Vector2 viewportSize)
    {
        float aspecto;
        if (viewportSize.h != 0)
            aspecto = static_cast<float>(viewportSize.w) / viewportSize.h;
        else
            aspecto = 1;
        glViewport(0, 0, viewportSize.w, viewportSize.h);
        glEnable(GL_DEPTH_TEST);

        cenaAtual()->renderizar(aspecto);
    }
    void SceneManager::atualizarCenaAtual(float deltaTime)
    {
        cenaAtual()->atualizar(deltaTime);
    }
    bool Scene::parse(rapidjson::Document& document)
    {
        // Verifica se o documento tem o membro "nome" e se é uma string
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
        // Analisa entidades
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
    bool Scene::existeEntidade(Entidades::Entidade* entidade) const {
        for (const auto& obj : Entidades) {
            if (obj.get() == entidade) {
                return true;
            }
        }
        return false;
    }
    int SceneManager::numeroDeCenas() const {
        return scenes.size();
    }
    int SceneManager::cenaAtualIdx() const {
        return currentSceneIndex;
    }
    std::string Scene::nome() const {
        return Name;
    }
    std::shared_ptr<Scene> SceneManager::cenaAtual() const {
        if (scenes.size() > 0)
            return scenes[currentSceneIndex];
        else
            Debug::emitir(Debug::Erro, "não tem cena atual calaio");
    }
}
