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
			Shader shader;
			const char* Nome = "componente_base";
			bool carregadov{ false };
		public:
			Bubble::Entidades::Entidade* meuObjeto = nullptr;
			Componente() {};
			const char* nome() const { return Nome; }
			bool carregado() const { return carregadov; };
			virtual void configurar() = 0; virtual void atualizar()  = 0;
			void definirPai(Bubble::Entidades::Entidade* ent) 
			{
				meuObjeto = ent;
			}
			void definirShader(const Shader& shade) {
				shader = shade;
			}
			virtual rapidjson::Value serializar(rapidjson::Document* doc) const
			{
				return rapidjson::Value();
			};
		};
	}
}	