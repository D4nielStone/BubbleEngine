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
			explicit Entidade(Bubble::Arquivadores::Arquivo3d arquivo_objeto);
			Entidade();
			~Entidade();
			Entidade(const char* name);
			void atualizar(float deltaTime);
			void renderizar();
			std::string* nome();
			void carregarModelo(Bubble::Arquivadores::Arquivo3d arquivo_objeto);
			std::shared_ptr<Bubble::Comum::Componente> obterComponente(const std::string& nome);
			std::vector<std::shared_ptr<Bubble::Comum::Componente>> obterComponentes(const std::string& nome);
			std::shared_ptr<Bubble::Componentes::Transformacao> obterTransformacao();
			const std::vector<std::shared_ptr<Bubble::Comum::Componente>>& listaDeComponentes() const;
			void adicionarComponente(std::shared_ptr<Bubble::Comum::Componente> componente);
			rapidjson::Value serializar(rapidjson::Document* a);
			bool parse(rapidjson::Value& v);
			bool ativado;
		private:
			std::shared_ptr<Bubble::Componentes::Transformacao> transformacao;
			std::vector<std::shared_ptr<Bubble::Comum::Componente>> Componentes;
			std::string Nome = "SemNome";
		};
	}
}