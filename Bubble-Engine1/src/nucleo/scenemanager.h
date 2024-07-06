#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H


#include "becore.h"
#include <vector>
#include <memory>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "src/entidades/entidade.h"
#include "src/arquivadores/shader.h"

namespace Bubble {
    namespace Nucleo {

        class BECORE_DLL_API Scene {
        private:
            const char* Name;
            std::vector<std::shared_ptr<Bubble::Entidades::Entidade>>
                Entidades; // vetor de objetos na cena

        public:
            Scene() : Name("Cena sem nome") {}

            ~Scene();

            explicit Scene(const char* name);

            void adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject);

            void atualizar(float deltaTime);

            void carregar();

            void serializar(rapidjson::Document* doc) const;

            std::string nome() const;

        private:
            bool existeEntidade(Entidades::Entidade* entidade) const;
        };

        class BECORE_DLL_API SceneManager {
        private:
            std::vector<std::shared_ptr<Scene>> 
                
                
                scenes; // vetor de cenas disponíveis
            int currentSceneIndex;                      // índice da cena atual

        public:
            SceneManager();

            ~SceneManager();

            int numeroDeCenas() const;

            void adicionarCena(std::shared_ptr<Scene> scene);

            int cenaAtual() const;

            void carregarCena(int sceneIndex);

            void atualizarCenaAtual(float deltaTime);
        };

    } // namespace Nucleo
} // namespace Bubble

#endif // SCENEMANAGER_H
