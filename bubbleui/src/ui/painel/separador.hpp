/*#pragma once
#include "src/ui/formas/linha.hpp"
#include "src/ui/formas/colisao2d.hpp"
#include "bubbleui.hpp"

namespace BubbleUI
{
	class Painel;
	class BEUI_DLL_API Separador
	{
	public:
		Separador() = default;
		Separador(const Lado side, Painel* painel);
		void atualizar();
		bool cursor() const;
	private:
		Colisao2d colisao;
		std::shared_ptr<Bubble::Inputs::Inputs> inputs{ nullptr };
		std::shared_ptr<Contexto> contexto{ nullptr };
		Painel* painel = nullptr;
		const Lado lado;
		bool mouse_1click, arrastando;
		Vector2 arrasto_pos, mouse_pos_ini;
		void atualizarColisao();
		void atualizarCursor();
		void atualizarArrasto();
	};
}
*/