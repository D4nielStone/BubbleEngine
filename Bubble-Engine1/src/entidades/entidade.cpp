#include "entidade.h"

namespace Bubble {
	namespace Entidades {
		Entidade::Entidade(Bubble::Arquivadores::Arquivo3d arquivo_objeto)
			: transformacao(nullptr) {
			adicionarComponente(std::make_shared<Bubble::Componentes::Transformacao>());
			carregarModelo(std::move(arquivo_objeto));
		}

		const char* Entidade::nome() {
			return Nome;
		}

		void Entidade::carregarModelo(Bubble::Arquivadores::Arquivo3d object_file) {
			int m = 0;
			for (const auto& vertex : object_file.vertices) {
				auto renderizador = std::make_shared<Bubble::Componentes::Renderizador>(vertex, object_file.materiais[m]);
				adicionarComponente(renderizador);
				m++;
			}
		}

		Bubble::Comum::Componente& Entidade::obterComponente(const std::string& nome) {
			for (auto& c : Componentes) {
				if (c->nome() == nome) {
					return *c;
				}
			}
			throw std::runtime_error("Componente não encontrado");
		}

		std::vector<std::reference_wrapper<Bubble::Comum::Componente>> Entidade::obterComponentes(const std::string& nome) {
			std::vector<std::reference_wrapper<Bubble::Comum::Componente>> comps;
			for (auto& c : Componentes) {
				if (c->nome() == nome) {
					comps.push_back(*c);
				}
			}
			return comps;
		}

		std::vector<Bubble::Comum::Componente*> Entidade::obterComponentesLogicos() {
			std::vector<Bubble::Comum::Componente*> comps;
			for (auto& c : ComponentesLogicos) {
				comps.push_back(c.get());
			}
			return comps;
		}

		Bubble::Componentes::Transformacao* Entidade::obterTransformacao() {
			return transformacao.get();
		}

		std::vector<Bubble::Componentes::Renderizador*> Entidade::obterRenderizadores() {
			std::vector<Bubble::Componentes::Renderizador*> renderers;
			for (auto& c : ComponentesGraficos) {
				if (auto render = std::dynamic_pointer_cast<Bubble::Componentes::Renderizador>(c)) {
					renderers.push_back(render.get());
				}
			}
			if (!renderers.empty())
				return renderers;
			throw std::runtime_error("Componente de Renderização não encontrado");
		}

		const std::vector<std::shared_ptr<Bubble::Comum::Componente>>& Entidade::listaDeComponentes() const {
			return Componentes;
		}

		template <typename T>
		void Entidade::adicionarComponente(std::shared_ptr<T> componente) {
			static_assert(std::is_base_of<Bubble::Comum::Componente, T>::value, "T must be derived from Componente");

			componente->definirPai(this);
			Componentes.push_back(componente);

			if (auto transform = std::dynamic_pointer_cast<Bubble::Componentes::Transformacao>(componente)) {
				transformacao = transform;
				ComponentesLogicos.push_back(transform);
			}
			else if (auto renderizador = std::dynamic_pointer_cast<Bubble::Componentes::Renderizador>(componente)) {
				ComponentesGraficos.push_back(renderizador);
			}
			else if (auto codigo = std::dynamic_pointer_cast<Bubble::Componentes::Codigo>(componente)) {
				ComponentesLogicos.push_back(codigo);
			}
			else if (auto camera = std::dynamic_pointer_cast<Bubble::Componentes::Camera>(componente)) {
				ComponentesLogicos.push_back(camera);
			}
		}

		// Explicit instantiation of the template function for allowed types
		template void Entidade::adicionarComponente<Bubble::Componentes::Transformacao>(std::shared_ptr<Bubble::Componentes::Transformacao>);
		template void Entidade::adicionarComponente<Bubble::Componentes::Renderizador>(std::shared_ptr<Bubble::Componentes::Renderizador>);
		template void Entidade::adicionarComponente<Bubble::Componentes::Codigo>(std::shared_ptr<Bubble::Componentes::Codigo>);
		template void Entidade::adicionarComponente<Bubble::Componentes::Camera>(std::shared_ptr<Bubble::Componentes::Camera>);
	}
}
