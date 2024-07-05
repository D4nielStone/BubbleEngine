#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>
#include <memory>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "src/entidades/entidade.h"
#include "src/arquivadores/shader.h"

namespace Bubble {
    namespace Nucleo {

        class Scene {
        private:
            const char* Name;
            std::vector<std::shared_ptr<Bubble::Entidades::Entidade>> Entidades; // vetor de objetos na cena

        public:

            Scene()
            {}

            Scene(const char* name) : Name(name) {
                std::cout << ">> " << name << " criada\n";
            }

            void adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject) {
                // Verifica se a entidade já existe na cena antes de adicionar
                if (!existeEntidade(gameObject.get())) {
                    std::cout << ">> Entidade " << gameObject->nome() << " adicionada à " << Name << "\n";
                    Entidades.push_back(gameObject);
                }
            }

            void atualizar(float deltaTime) {
                glEnable(GL_DEPTH_TEST);
                glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
                glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
                for (auto& obj : Entidades) {
                    obj->atualizar(deltaTime);
                }
            }

            void carregar() {
                std::cout << ">> Carregando shader Phong" << std::endl;
                auto phong = new Shader("assets/shaders/phong.vert", "assets/shaders/phong.frag");
                phong->use();
                std::cout << ">> Shader carregado e usado." << std::endl;

                for (auto& obj : Entidades) {
                    std::cout << "\n\n>> Configurando entidade: " << obj->nome() << std::endl;
                    for (auto& c : obj->listaDeComponentes()) {
                        c->definirShader(phong);
                        std::cout << ">> Shader definido para o componente." << std::endl;
                        c->configurar();
                        std::cout << ">> Componente configurado.\n\n" << std::endl;
                    }
                }
            }
            void serializar(rapidjson::Document* doc) const
            {
                doc->SetObject();
                doc->AddMember("nome", rapidjson::Value().SetString(Name, doc->GetAllocator()),doc->GetAllocator());
                
                rapidjson::Value arr(rapidjson::kArrayType);
                for (auto& entidade : Entidades)
                {
                    arr.PushBack(entidade->serializar(doc), doc->GetAllocator());
                }

                doc->AddMember("entidades", arr,doc->GetAllocator());
            }

            std::string nome()
            {
                return Name;
            }
        private:
            bool existeEntidade(Entidades::Entidade* entidade) {
                for (const auto& obj : Entidades) {
                    if (obj.get() == entidade) {
                        return true;
                    }
                }
                return false;
            }

            // Métodos adicionais conforme necessário para gerenciar a cena, como adicionar/remover objetos, etc.
        };


        class SceneManager {
        private:
            std::vector<std::shared_ptr<Scene>> scenes; // vetor de cenas disponíveis
            int currentSceneIndex;                      // índice da cena atual

        public:
            SceneManager() : currentSceneIndex(-1) {}
            


            int numeroDeCenas() const
            {
                return scenes.size();
            }

            void adicionarCena(std::shared_ptr<Scene> scene) {
                scenes.push_back(scene);
                currentSceneIndex = scenes.size() - 1;
            }

            int cenaAtual()
            {
                return currentSceneIndex;
            }

            void carregarCena(int sceneIndex) {
                if (sceneIndex >= 0 && sceneIndex < scenes.size()) {
                    currentSceneIndex = sceneIndex;
                    std::cout << "Carregando cena: " << sceneIndex << std::endl;
                    scenes[currentSceneIndex]->carregar();
                }
                else {
                    std::cerr << "Erro: índice da cena não é válido!" << std::endl;
                }
            }

            void atualizarCenaAtual(float deltaTime) {
                if (currentSceneIndex >= 0 && currentSceneIndex < scenes.size()) {
                    scenes[currentSceneIndex]->atualizar(deltaTime);
                }
            }

            // Métodos adicionais conforme necessário, como transição suave entre cenas, etc.
        };

    } // namespace Nucleo
} // namespace Bubble

#endif // SCENEMANAGER_H
