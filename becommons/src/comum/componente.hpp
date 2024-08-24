#pragma once
#include "becommons.hpp"
#include "src/arquivadores/shader.hpp"
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"

namespace Bubble {
	namespace Entidades {
		class Entidade;
		// Declaração antecipada da classe Entidade
	}
	namespace Comum {

		class BECOMMONS_DLL_API Componente {
		protected:
			Shader* shader = new Shader();
			Bubble::Entidades::Entidade* meuObjeto = nullptr;
			const char* Nome = "componente_base";
			bool carregadov{ false };
		public:
			Componente() {};
			const char* nome() const { return Nome; }
			bool carregado() const { return carregadov; };
			virtual void configurar() = 0; virtual void atualizar(float deltaTime = 0)  = 0;
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