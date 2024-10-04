#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include "Scene.hpp"

namespace Bubble {
    namespace Cena {
        class BECOMMONS_DLL_API SceneManager {
        private:
            std::vector<Scene*> scenes; 
            int currentSceneIndex;
            Scene* criarCenaPadrao(std::string Nome);
            std::shared_ptr<Inputs::Inputs> inputs{ nullptr };
            // viewport para renderizacao
            Vector4 viewportEditor, viewportJogo;
        public:
            SceneManager();
            ~SceneManager();
            void defIputs(std::shared_ptr<Inputs::Inputs>);
            size_t numeroDeCenas() const;
            void adicionarCena(Scene* scene);
            int cenaAtualIdx() const;
            Scene* cenaAtual() const;
            void carregarCena(int sceneIndex);
            void renderizarCenaAtual() const;
            void atualizarCenaAtual() const;
            void novaCena(std::string Nome = "Cena nova", bool cenaPadrao = true);
            void defEditorViewport(Vector4 rect = {0, 0, 0, 0});
            void defJogoViewport(Vector4 rect = {0, 0, 0, 0});
        };
        extern BECOMMONS_DLL_API Bubble::Entidades::CameraEditor* CameraEditorAtual();
        extern BECOMMONS_DLL_API std::shared_ptr<Bubble::Cena::SceneManager> obterSceneManager();
        extern BECOMMONS_DLL_API void definirSceneManager(std::shared_ptr<SceneManager> scene_manager);
        BECOMMONS_DLL_API void adicionarTarefaNaFila(std::function<void()> tarefa);
        BECOMMONS_DLL_API void criarEntidade(std::string path);
        BECOMMONS_DLL_API void criarCamera(glm::vec3 posicao);
        BECOMMONS_DLL_API void adicionarCena();
    } // namespace Cena
} // namespace Bubble