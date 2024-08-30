#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include "Scene.hpp"

enum class BECOMMONS_DLL_API Modo
{
    Editor,
    Jogo
};

namespace Bubble {
    namespace Cena {
        class BECOMMONS_DLL_API SceneManager {
        private:
            std::vector<Scene*> scenes; 
            int currentSceneIndex;
            Scene* criarCenaPadrao(std::string Nome);
            Inputs::Inputs* inputs = nullptr;
            Vector4 viewport_rect;
        public:
            SceneManager();
            ~SceneManager();
            void defIputs(Inputs::Inputs*);
            size_t numeroDeCenas() const;
            void adicionarCena(Scene* scene);
            int cenaAtualIdx() const;
            Scene* cenaAtual() const;
            void carregarCena(int sceneIndex);
            void renderizarCenaAtual() const;
            void atualizarCenaAtual() const;
            void novaCena(std::string Nome = "Cena nova", bool cenaPadrao = true);
            void defViewport(Vector4 rect = {0, 0, 0, 0});
        };
        BECOMMONS_DLL_API void adicionarTarefaNaFila(std::function<void()> tarefa);
        BECOMMONS_DLL_API void criarEntidade(SceneManager* scenemanager, std::string path);
    } // namespace Cena
} // namespace Bubble