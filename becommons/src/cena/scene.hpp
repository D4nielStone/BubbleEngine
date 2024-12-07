#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "src/entidades/entidade.hpp"
#include "src/arquivadores/shader.hpp"
#include "src/entidades/camera_editor.hpp"
#include "becommons.hpp"

namespace Bubble
{
    namespace Cena
    {
        inline Bubble::Entidades::CameraEditor camera_editor;
        class BECOMMONS_DLL_API PipeLine
        {
        private:
            std::unordered_map<size_t, std::pair<std::vector<std::shared_ptr<Entidades::Entidade>>, Material*>> entidadesParaRenderizar;
        public:
            PipeLine() = default;
            void adicionarEntidade(std::shared_ptr<Entidades::Entidade> ent);
            void removerEntidade(std::shared_ptr<Entidades::Entidade> ent);
            void renderizar(Componentes::Camera* cam);
        };
        class BECOMMONS_DLL_API Scene
        {
        private:
            std::shared_ptr<std::string> Name = std::make_shared<std::string>("Cena");
            bool existeEntidade(Entidades::Entidade* entidade) const;
            bool entidadeExisteRecursivo(std::shared_ptr<Entidades::Entidade> obj, Entidades::Entidade* entidade) const;
        public:
            PipeLine renderizadores;
            std::shared_ptr<Entidades::Entidade> entidade_selecionada{ nullptr };
            std::vector<std::shared_ptr<Bubble::Entidades::Entidade>>Entidades;
            Bubble::Componentes::Camera* camera_principal = nullptr;

            Scene() {}
            Scene(const std::string &name);
            ~Scene();
            std::shared_ptr<Entidades::Entidade> criarEntidade(const std::string &path, const char* nome_entidade = "entidade sem nome");
            void adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject);
            void removerEntidade(std::shared_ptr<Entidades::Entidade> gameObject);
            void renderizar(const InputMode modo);
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