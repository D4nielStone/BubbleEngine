#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "becore.h"
#include <vector>
#include <memory>
#include <iostream>
#include "Scene.hpp"

enum class BECORE_DLL_API Modo
{
    Editor,
    Jogo
};

namespace Bubble {
    namespace Nucleo {
        class BECORE_DLL_API SceneManager {
        private:
            std::vector<Scene*> scenes; 
            int currentSceneIndex;
            Scene* criarCenaPadrao(std::string Nome);
            Inputs::Inputs* inputs = nullptr;
        public:
            SceneManager();
            ~SceneManager();
            void defIputs(Inputs::Inputs*);
            int numeroDeCenas() const;
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

#endif // SCENEMANAGER_H
