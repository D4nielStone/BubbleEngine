#include "scenemanager.h"
#include "src/debug/debug.h"

std::shared_ptr<Shader>phong;
namespace Bubble {

    namespace Nucleo {

        Scene::Scene(const char* name) : Name(name) {
            std::string msg = std::string(name) + " criada";
            Debug::emitir(Tipo::Mensagem, msg.c_str());
        }
        
        Scene::~Scene() {}

        void Scene::adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject) {
            // Verifica se a entidade já existe na cena antes de adicionar
            if (!existeEntidade(gameObject.get())) {
                Entidades.push_back(gameObject);
            }
        }

        void Scene::atualizar(Modo m, float deltaTime, float aspecto) {
            glEnable(GL_DEPTH_TEST);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
            for (auto& obj : Entidades) {
                obj->atualizar(m, deltaTime, aspecto);
            }
        }

        void Scene::
            carregar() {

            for (auto& obj : Entidades) {
                for (auto& c : obj->listaDeComponentes()) {
                    c->definirShader(phong.get());
                    
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

        int SceneManager::cenaAtual() const {
            return currentSceneIndex;
        }

        void SceneManager::carregarCena(int sceneIndex) {
            phong = std::make_shared<Shader>("assets/shaders/phong.vert", "assets/shaders/phong.frag");
            phong->use();
            camera_do_editor.definirShader(phong.get());
            if (sceneIndex >= 0 && sceneIndex < scenes.size()) {
                currentSceneIndex = sceneIndex;

                std::string msg("Carregando cena ");
                std::string soma = msg + std::to_string(currentSceneIndex);

                Debug::emitir(Tipo::Mensagem, soma.c_str());
                
                scenes[currentSceneIndex]->carregar();
            }
            else {
                std::cerr << "Erro: índice da cena não é válido!" << std::endl;
            }
        }

        void SceneManager::atualizarCenaAtual(Modo m, float deltaTime, float aspecto) {
            if(m == Modo::Editor)
            {
                camera_do_editor.atualizarAspecto(aspecto);
                camera_do_editor.atualizar(deltaTime);
            }
            if (currentSceneIndex >= 0 && currentSceneIndex < scenes.size()) 
            {
                scenes[currentSceneIndex]->atualizar(m, deltaTime, aspecto);
            }
        }

    } // namespace Nucleo
} // namespace Bubble
