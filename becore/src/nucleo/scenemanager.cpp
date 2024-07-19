#include "scenemanager.h"
#include "becore.h"
#include "src/depuracao/debug.h"

namespace Bubble {

    namespace Nucleo {

        Scene::Scene(const char* name) : Name(name) {
            std::string msg = std::string(name) + " criada";
            Debug::emitir("CENA", msg.c_str());
        }
        
        Scene::~Scene() {}

        void Scene::
            adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject) {
            if (!existeEntidade(gameObject.get())) {
                for (auto& c : gameObject->listaDeComponentes()) {
                    c->configurar();
                }
                Entidades.push_back(gameObject);
            }
        }

        void Scene::desenharCeu()
        {
        }
        
        void Scene::atualizar(Modo m, float deltaTime, float aspecto) {

            desenharCeu();

            // Atualizar a câmera
            if (m == Modo::Editor) {
                camera_editor.atualizarAspecto(aspecto);
                camera_editor.atualizar(deltaTime);
            }
            else if (camera_principal)
            {
                camera_principal->atualizarAspecto(aspecto);
                camera_principal->atualizar(deltaTime);
            }

            for (auto& obj : Entidades) {
                obj->atualizar(m, deltaTime, aspecto);
                if (obj->obterComponente("Camera"))
                {
                    camera_principal = dynamic_cast<Bubble::Componentes::Camera*>(obj->obterComponente("Camera").get());
                }
            }
        }

        void Scene::
            carregar() {
            entidadeSelecionada = Entidades[Entidades.size() - 1].get();
            camera_editor.configurar();

            glEnable(GL_DEPTH_TEST);
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

        std::string Scene::nome() const {
            return Name;
        }

        bool Scene::existeEntidade(Entidades::Entidade* entidade) const {
            for (const auto& obj : Entidades) {
                if (obj.get() == entidade) {
                    return true;
                }
            }
            return false;
        }

        SceneManager::SceneManager() : currentSceneIndex(-1) 
        {
        }
        
        SceneManager::~SceneManager() {}

        int SceneManager::numeroDeCenas() const {
            return scenes.size();
        }

        void SceneManager::adicionarCena(std::shared_ptr<Scene> scene) {
            scenes.push_back(scene);
        }

        int SceneManager::cenaAtualIdx() const {
            return currentSceneIndex;
        }
        
        std::shared_ptr<Scene> SceneManager::cenaAtual() const {
            return scenes[currentSceneIndex];
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
                Debug::emitir(Debug::Tipo::Erro,"índice da cena não é válido");
            }
        }

        void SceneManager::atualizarCenaAtual(Modo m, float deltaTime, int window_x, int window_y, int fb_w, int fb_h)
        {
            float aspecto = static_cast<float>(fb_w) / fb_h;
            
            glViewport(0, 0, fb_w, fb_h);

            // Update the current scene if it exists
            if (currentSceneIndex >= 0 && currentSceneIndex < scenes.size()) {
                scenes[currentSceneIndex]->atualizar(m, deltaTime, aspecto);
            }
        }
    } // namespace Nucleo
} // namespace Bubble
