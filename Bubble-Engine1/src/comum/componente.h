#ifndef COMPONENTE_H
#define COMPONENTE_H
#include "src/arquivadores/shader.h"
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"

namespace Bubble {
	namespace Entidades {
		class Entidade;
		// Declaração antecipada da classe Entidade
	}
	namespace Comum {

		class Componente {
		protected:
			Shader* shader = nullptr;
			Bubble::Entidades::Entidade* meuObjeto = nullptr;
			const char* Nome = "componente_base";
		public:
			Componente() {};
			const char* nome() const { return Nome; }
			virtual void configurar() = 0; virtual void atualizar(float deltaTime) = 0;
			void definirPai(Bubble::Entidades::Entidade* ent) {
				meuObjeto = ent;
			}
			void definirShader(Shader* shade) {
				shader = shade;
			}
			virtual rapidjson::Value serializar(rapidjson::Document* doc)
			{
				return rapidjson::Value();
			};
		};
	}
}	
#endif //!COMPONENTE_H