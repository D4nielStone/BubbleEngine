#pragma once
#include "becommons.hpp"
#include <rapidjson/document.h>
#include <rapidjson/allocators.h>
#include <any>
#include <variant>
#include <vector>

namespace Bubble {
	namespace Entidades {
		class Entidade;	// Declaração antecipada da classe Entidade
	}
	namespace Comum {

		class BECOMMONS_DLL_API Componente {
		protected:
			const char* Nome = "componente_base";
			bool carregadov{ false };
		public:
			std::vector<std::any> variaveis;
			Bubble::Entidades::Entidade* meuObjeto = nullptr;
			Componente() {};
			const char* nome() const { return Nome; }
			bool carregado() const { return carregadov; };
			virtual void configurar() = 0; virtual void atualizar()  = 0;
			void definirPai(Bubble::Entidades::Entidade* ent) 
			{
				meuObjeto = ent;
			}
			virtual rapidjson::Value serializar(rapidjson::Document* doc) const
			{
				return rapidjson::Value();
			};
		};
	}
}	