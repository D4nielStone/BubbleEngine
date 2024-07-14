#ifndef ENTIDADE_H
#define ENTIDADE_H

#include "becore.h"
#include "src/comum/componente.h"
#include "src/arquivadores/arquivo3d.h"
#include "src/componentes/transformacao/transformacao.h"
#include "src/componentes/renderizador/renderizador.h"
#include "src/componentes/codigo/codigo.h"
#include "src/componentes/camera/camera.h"
#include <memory>
#include <vector>
#include <stdexcept>
#include "rapidjson/document.h"

enum class Modo;

namespace Bubble {
	namespace Entidades {
		class BECORE_DLL_API Entidade {
		public:
			Entidade();

			Entidade(const char* name);

			~Entidade();

			explicit Entidade(Bubble::Arquivadores::Arquivo3d arquivo_objeto);

			void atualizar(Modo m, float deltaTime, float aspecto);

			const char* nome();

			void carregarModelo(Bubble::Arquivadores::Arquivo3d arquivo_objeto);

			std::shared_ptr<Bubble::Comum::Componente> obterComponente(const std::string& nome);

			std::vector<std::shared_ptr<Bubble::Comum::Componente>> obterComponentes(const std::string& nome);


			std::shared_ptr<Bubble::Componentes::Transformacao> obterTransformacao();

			const std::vector<std::shared_ptr<Bubble::Comum::Componente>>& listaDeComponentes() const;

			void adicionarComponente(std::shared_ptr<Bubble::Comum::Componente> componente);
			
			rapidjson::Value serializar(rapidjson::Document* a);

		private:
			std::shared_ptr<Bubble::Componentes::Transformacao> transformacao;
			std::vector<std::shared_ptr<Bubble::Comum::Componente>> Componentes;

			const char* Nome = "NONAMEENTITIE";
		};
	}
}
#endif //!ENTIDADE_H