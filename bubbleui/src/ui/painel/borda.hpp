#pragma once
#include "src/ui/formas/linha.hpp"
#include "src/ui/formas/colisao2d.hpp"
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
		bool cursor();
	private:
		Colisao2d* colisao = nullptr;
		Formas::Linha* linha = nullptr;
		Painel* painel = nullptr;
		Lado lado;
		void atualizarColisao();
		void atualizarCursor();
	};
}