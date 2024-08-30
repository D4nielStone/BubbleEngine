#pragma once
#include "becommons.hpp"
#include "src/comum/componente.hpp"
#include "src/arquivadores/arquivo3d.hpp"
#include "src/componentes/transformacao/transformacao.hpp"
#include "src/componentes/renderizador/renderizador.hpp"
#include "src/componentes/codigo/codigo.hpp"
#include "src/componentes/camera/camera.hpp"
#include <memory>
#include <vector>
#include <stdexcept>
#include "rapidjson/document.h"

namespace Bubble {
	namespace Entidades {
		class BECOMMONS_DLL_API Entidade {
		public:
			Entidade(const Arquivadores::Arquivo3d& arquivo_objeto);
			Entidade(const char* name);
			Entidade();
			~Entidade();
			void atualizar() const;
			void renderizar() const;
			std::string nome() const;
			void carregarNode(const Node& node);
			std::shared_ptr<Comum::Componente> obterComponente(const std::string& nome);
			std::unordered_set<std::shared_ptr<Comum::Componente>> obterComponentes(const std::string& nome);
			const std::vector<std::shared_ptr<Entidade>>& obterFilhos() const;
			std::shared_ptr<Componentes::Transformacao> obterTransformacao() const;
			const std::unordered_set<std::shared_ptr<Comum::Componente>>& listaDeComponentes() const;
			void adicionarComponente(std::shared_ptr<Comum::Componente> componente);
			rapidjson::Value serializar(rapidjson::Document* a);
			bool parse(rapidjson::Value& v);
			bool ativado;
			Entidade* pai{ nullptr };
		private:
			std::shared_ptr<Componentes::Transformacao> transformacao;
			std::unordered_set<std::shared_ptr<Comum::Componente>> Componentes;
			std::vector<std::shared_ptr<Entidade>> filhos;
			std::string Nome = "SemNome";
		};
	}
}