#ifndef ENTIDADE_H
#define ENTIDADE_H

#include "src/comum/componente.h"
#include "src/arquivadores/arquivo3d.h"
#include "src/componentes/transformacao/transformacao.h"
#include "src/componentes/renderizador/renderizador.h"
#include "src/componentes/codigo/codigo.h"
#include "src/componentes/camera/camera.h"
#include <memory>
#include <vector>
#include <stdexcept>

namespace Bubble {
	namespace Entidades {
		class Entidade {
		public:
			Entidade() {
				adicionarComponente(std::make_shared<Bubble::Componentes::Transformacao>());
			};
			explicit Entidade(Bubble::Arquivadores::Arquivo3d arquivo_objeto);

			const char* nome();

			void carregarModelo(Bubble::Arquivadores::Arquivo3d arquivo_objeto);

			Bubble::Comum::Componente& obterComponente(const std::string& nome);

			std::vector<std::reference_wrapper<Bubble::Comum::Componente>> obterComponentes(const std::string& nome);

			std::vector<Bubble::Comum::Componente*> obterComponentesLogicos();

			Bubble::Componentes::Transformacao* obterTransformacao();

			std::vector<Bubble::Componentes::Renderizador*> obterRenderizadores();

			const std::vector<std::shared_ptr<Bubble::Comum::Componente>>& listaDeComponentes() const;

			template <typename T>
			void adicionarComponente(std::shared_ptr<T> componente);

		private:
			std::shared_ptr<Bubble::Componentes::Transformacao> transformacao;
			std::vector<std::shared_ptr<Bubble::Comum::Componente>> Componentes;
			std::vector<std::shared_ptr<Bubble::Comum::Componente>> ComponentesLogicos;
			std::vector<std::shared_ptr<Bubble::Comum::Componente>> ComponentesGraficos;

			const char* Nome = "NONAMEENTITIE";
		};
	}
}
#endif //!ENTIDADE_H