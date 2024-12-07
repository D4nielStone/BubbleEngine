#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include "Scene.hpp"

namespace Bubble {
    namespace Cena {
        class BECOMMONS_DLL_API SceneManager {
        private:
            std::vector<std::shared_ptr<Scene>> scenes; 
            int currentSceneIndex;
            std::shared_ptr<Scene> criarCenaPadrao(std::string Nome);
            std::shared_ptr<Inputs::Inputs> inputs{ nullptr };
            // viewport para renderizacao
            Vector4<int> viewportEditor, viewportJogo;
        public:
            SceneManager();
            ~SceneManager();
            std::vector<std::shared_ptr<Scene>> obterCenas() const;
            void defIputs(std::shared_ptr<Inputs::Inputs>);
            size_t numeroDeCenas() const;
            void adicionarCena(std::shared_ptr<Scene> scene);
            int cenaAtualIdx() const;
            std::shared_ptr<Scene> cenaAtual() const;
            void carregarCena(int sceneIndex);
            void renderizarCenaAtual() const  ;
            void atualizarCenaAtual() const;
            void novaCena(std::string Nome = "Cena nova", bool cenaPadrao = true);
            void defEditorViewport(Vector4<int> rect = {0, 0, 0, 0});
            void defJogoViewport(Vector4<int> rect = {0, 0, 0, 0});
        };
        extern Bubble::Entidades::CameraEditor* CameraEditorAtual();
        extern std::shared_ptr<Bubble::Cena::SceneManager> obterSceneManager();
        extern void definirSceneManager(std::shared_ptr<SceneManager> scene_manager);
        void criarEntidade(std::string path);
        void criarCamera(const Vector3<float> &posicao);
        void adicionarCena();
    } // namespace Cena
} // namespace Bubble