#include <vector>
#include <memory>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "src/entidades/entidade.hpp"
#include "src/arquivadores/shader.hpp"
#include "src/entidades/camera_editor.hpp"
#include "src/util/skybox.hpp"
#include "becommons.hpp"

namespace Bubble {
    namespace Cena {

        class BECOMMONS_DLL_API Scene {
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

            float corCeu[3]{ 0.3f, 0.3f, 1.f };

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
    }
}