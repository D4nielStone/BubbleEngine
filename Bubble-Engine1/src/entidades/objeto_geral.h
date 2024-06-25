#pragma once
#include "src/comum/componente.h"
#include "src/arquivadores/arquivo3d.h"
#include "src/componentes/transformador.h"
#include "src/componentes/renderizador.h"
#include <memory>
#include <vector>
#include <stdexcept>

namespace Bubble {
	namespace Entidades {
		class ObjetoGeral {
		public:
			explicit ObjetoGeral(Bubble::Arquivadores::Arquivo3d arquivo_objeto) {
				carregarModelo(std::move(arquivo_objeto));
			}

			void carregarModelo(Bubble::Arquivadores::Arquivo3d arquivo_objeto);

			Bubble::Comum::Componente& obterComponente(const std::string& nome) {
				for (auto& c : Componentes) {
					if (c->nome() == nome) {
						return *c;
					}
				}
				throw std::runtime_error("Componente não encontrado");
			}

			std::vector<std::reference_wrapper<Bubble::Comum::Componente>> obterComponentes(const std::string& nome) {
				std::vector<std::reference_wrapper<Bubble::Comum::Componente>> comps;
				for (auto& c : Componentes) {
					if (c->nome() == nome) {
						comps.push_back(*c);
					}
				}
				return comps;
			}

			Bubble::Componentes::Transformador* obterTransformador() {
				for (auto& c : Componentes) {
					if (c->nome() == "Transformador") {
						if (auto transform = dynamic_cast<Bubble::Componentes::Transformador*>(c.get())) {
							return transform;
						}
					}
				}
				throw std::runtime_error("Componente de Transformação não encontrado");
			}

			Bubble::Componentes::Rendererizador* obterRenderizador() {
				for (auto& c : Componentes) {
					if (c->nome() == "Renderizador") {
						if (auto render = dynamic_cast<Bubble::Componentes::Rendererizador*>(c.get())) {
							return render;
						}
					}
				}
				throw std::runtime_error("Componente de Transformação não encontrado");
			}

			const std::vector<std::unique_ptr<Bubble::Comum::Componente>>& listaDeComponentes() const {
				return Componentes;
			}
		private:
			std::vector<std::unique_ptr<Bubble::Comum::Componente>> Componentes;
		};
	}
}
