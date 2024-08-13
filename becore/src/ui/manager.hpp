#pragma once
#include "src/ui/painel/painel.hpp"
#include "src/ui/contexto/contexto.hpp"

namespace Bubble
{
	namespace Nucleo
	{
		class Engine;
	}
}
namespace BubbleUI
{
	class Manager
	{
	private:
		std::vector<Painel*> lista_paineis;
		Bubble::Nucleo::Engine* engine = nullptr;
		Contexto contexto;
		Colisao2d colisao_painel;
		void iniPaineisPadrao();
		void painelSelecionado(Painel* painel);
	public:
		Manager(Bubble::Nucleo::Engine*);
		void atualizar(float deltaTime);
		void renderizar();
	};
}
