#include "scenemanager.h"
#include "becore.h"
#include "src/depuracao/debug.h"

std::shared_ptr<Shader>phong;
namespace Bubble {

    namespace Nucleo {

        Scene::Scene(const char* name) : Name(name) {
            std::string msg = std::string(name) + " criada";
            Debug::emitir(Debug::Tipo::Mensagem, msg.c_str());
        }
        
        Scene::~Scene() {}

        void Scene::
            adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject) {
            if (!existeEntidade(gameObject.get())) {
                for (auto& c : gameObject->listaDeComponentes()) {
                    c->definirShader(phong.get());

                    c->configurar();
                }
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

        int SceneManager::cenaAtualIdx() const {
            return currentSceneIndex;
        }
        
        std::shared_ptr<Scene> SceneManager::cenaAtual() const {
            return scenes[currentSceneIndex];
        }

        void SceneManager::carregarCena(int sceneIndex) {
            phong = std::make_shared<Shader>("assets/shaders/phong.vert", "assets/shaders/phong.frag");
            phong->use();
            camera_do_editor.configurar();
            camera_do_editor.definirShader(phong.get());
            if (sceneIndex >= 0 && sceneIndex < scenes.size()) {
                currentSceneIndex = sceneIndex;

                std::string msg("Carregando cena ");
                std::string soma = msg + std::to_string(currentSceneIndex);

                Debug::emitir(Debug::Tipo::Mensagem, soma.c_str());
                
                scenes[currentSceneIndex]->carregar();
            }
            else {
                Debug::emitir(Debug::Tipo::Erro,"�ndice da cena n�o � v�lido");
            }
        }

        void SceneManager::atualizarCenaAtual(Modo m, float deltaTime, int window_w, int window_h, int fb_w, int fb_h)
        {
            float aspecto = (m == Modo::Editor) ? static_cast<float>(window_w) / window_h : static_cast<float>(fb_w) / fb_h;

            // Ajustar o viewport para centralizar a imagem
            int width, height;
            if (m == Modo::Editor) {
                width = window_w;
                height = window_h;
            }
            else {
                width = fb_w;
                height = fb_h;
            }

            // Calcular as dimens�es do viewport centralizado
            int viewportWidth = static_cast<int>(height * aspecto);
            int viewportHeight = height;

            if (viewportWidth > width) {
                viewportWidth = width;
                viewportHeight = static_cast<int>(width / aspecto);
            }

            int viewportX = (width - viewportWidth) / 2;
            int viewportY = (height - viewportHeight) / 2;

            // Definir o viewport
            glViewport(viewportX, viewportY, viewportWidth, viewportHeight);

            // Atualizar a c�mera
            if (m == Modo::Editor) {
                camera_do_editor.atualizarAspecto(aspecto);
                camera_do_editor.atualizar(deltaTime);
            }

            if (currentSceneIndex >= 0 && currentSceneIndex < scenes.size()) {
                scenes[currentSceneIndex]->atualizar(m, deltaTime, aspecto);
            }
        }
    


    } // namespace Nucleo
} // namespace Bubble