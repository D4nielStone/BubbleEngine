#pragma once
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

namespace Bubble
{
    namespace Cena
    {
        class BECOMMONS_DLL_API Scene
        {
        private:
            Shader shader;
            std::shared_ptr<std::string> Name = std::make_shared<std::string>("Cena");
            std::unordered_map<std::string, std::pair<std::vector<std::shared_ptr<Entidades::Entidade>>, Material>> entidadesParaRenderizar;
            std::unique_ptr<Bubble::Util::Skybox> skybox{ nullptr };
            bool existeEntidade(Entidades::Entidade* entidade) const;
            bool entidadeExisteRecursivo(std::shared_ptr<Entidades::Entidade> obj, Entidades::Entidade* entidade) const;
        public:
            std::shared_ptr<Entidades::Entidade> entidade_selecionada{ nullptr };
            std::vector<std::shared_ptr<Bubble::Entidades::Entidade>>Entidades;
            Bubble::Entidades::CameraEditor camera_editor;
            Bubble::Componentes::Camera* camera_principal = nullptr;

            float corCeu[3]{ 0.3f, 0.3f, 1.f };

            Scene() {}
            Scene(const std::string &name);
            ~Scene();
            void carregarComponentes(std::shared_ptr<Entidades::Entidade> entidade);
            bool criarEntidade(const std::string &path, const char* nome_entidade = "entidade sem nome");
            void adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject);
            void renderizar(const InputMode modo) const;
            void atualizar(float aspectoDoEditor, float aspectoDoJogo);
            void atualizarFilhos(std::shared_ptr<Entidades::Entidade> entidade);
            void carregar();
            void serializar(rapidjson::Document* doc) const;
            bool parse(rapidjson::Document& doc);
            std::string nome() const;
            std::shared_ptr<std::string> nomeptr();
            std::pair<std::future<void>, Arquivadores::Arquivo3d>tarefaCriarEntidade;
        };
    }
}