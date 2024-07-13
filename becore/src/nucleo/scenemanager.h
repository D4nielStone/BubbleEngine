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
        public:
            std::vector<std::shared_ptr<Bubble::Entidades::Entidade>>Entidades;
            Bubble::Entidades::CameraEditor camera_editor;
            Bubble::Componentes::Camera* camera_principal = nullptr;
            Scene() : Name("Cena sem nome") {}

            ~Scene();

            explicit Scene(const char* name);

            void adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject);

            void atualizar(Modo m, float deltaTime,float aspecto);

            void carregar();

            void serializar(rapidjson::Document* doc) const;

            std::string nome() const;

        private:
            bool existeEntidade(Entidades::Entidade* entidade) const;
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

            void atualizarCenaAtual(Modo m, float deltaTime, int window_w, int window_h, int fb_w, int fb_h);
        };

    } // namespace Nucleo
} // namespace Bubble

#endif // SCENEMANAGER_H
