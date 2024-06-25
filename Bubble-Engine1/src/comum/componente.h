#pragma once
namespace Bubble {
	namespace Comum {
		class Componente {
		protected:
			const char* Nome = "NoName_Component";
		public:
			Componente() {};
			const char* nome() const { return Nome; }
			virtual void configurar() = 0; virtual void atualizar() = 0;
		};
	}
}