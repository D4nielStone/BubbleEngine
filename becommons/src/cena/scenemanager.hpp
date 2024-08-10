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
        public:
            SceneManager();
            ~SceneManager();
            void defIputs(Inputs::Inputs*);
            size_t numeroDeCenas() const;
            void adicionarCena(Scene* scene);
            int cenaAtualIdx() const;
            Scene* cenaAtual() const;
            void carregarCena(int sceneIndex);
            void renderizarCenaAtual(Vector4 viewportSize) const;
            void atualizarCenaAtual(float deltaTime) const;
            void novaCena(std::string Nome = "Cena nova", bool cenaPadrao = true);
        };

    } // namespace Nucleo
} // namespace Bubble