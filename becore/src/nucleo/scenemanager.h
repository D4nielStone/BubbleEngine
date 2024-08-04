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
#include "src/entidades/camera_editor.h"
#include "src/util/skybox.h"

enum class BECORE_DLL_API Modo
{
    Editor,
    Jogo
};

namespace Bubble {
    namespace Nucleo {

        class BECORE_DLL_API Scene {
        private:
            const char* Name;
            Bubble::Util::Skybox skybox;

            bool existeEntidade(Entidades::Entidade* entidade) const;
            void desenharCeu();
        public:
            std::vector<std::shared_ptr<Bubble::Entidades::Entidade>>Entidades;
            Entidades::Entidade* entidadeSelecionada = nullptr;
            Bubble::Entidades::CameraEditor camera_editor;
            Bubble::Componentes::Camera* camera_principal = nullptr;

            float corCeu[3]{0.3f, 0.3f, 1.f};

            Scene() : Name("Cena sem nome") {}
            Scene(const char* name);
            ~Scene();
            void adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject);
            void renderizar(float aspecto);
            void atualizar(float deltaTime);
            void carregar();
            void serializar(rapidjson::Document* doc) const;
            bool parse(rapidjson::Document& doc);
            std::string nome() const;
        };

        class BECORE_DLL_API SceneManager {
        private:
            std::vector<std::shared_ptr<Scene>> scenes; 
            int currentSceneIndex;

        public:
            SceneManager();

            ~SceneManager();

            int numeroDeCenas() const;

            void adicionarCena(std::shared_ptr<Scene> scene);

            int cenaAtualIdx() const;
            
            std::shared_ptr<Scene> cenaAtual() const;

            void carregarCena(int sceneIndex);

            void renderizarCenaAtual(Vector2 viewportSize);
            void atualizarCenaAtual(float deltaTime);
        };

    } // namespace Nucleo
} // namespace Bubble

#endif // SCENEMANAGER_H
