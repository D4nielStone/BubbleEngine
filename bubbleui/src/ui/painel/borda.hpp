#pragma once
#include "src/ui/formas/linha.hpp"
#include "bubbleui.hpp"

enum Lado
{
	DIREITA, ESQUERDA, CIMA, BAIXO
};
namespace BubbleUI
{
	class Painel;
	class BEUI_DLL_API Borda
	{
	public:
		Borda() = default;
		Borda(Lado side, Painel* painel);
		void atualizar(float deltaTime);
		void renderizar();
	private:
		Formas::Linha* linha;
		Painel* painel = nullptr;
		Lado lado;
	};
}