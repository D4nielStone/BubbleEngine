#pragma once
#include "ancora.hpp"
#include "src/ui/formas/colisao2d.hpp"

namespace BubbleUI
{
	class Separador
	{
	public:
		/**
		 * @brief Construtor do separador
		 * @param tipo Tipo do separador (Vertical ou Horizontal)
		 * @param ancora Ancora pai para redimesionamento
		 */
		Separador(TipoAncoragem tipo, Ancora* ancora);

		/**
		 * @brief Proporcao de divisao entre paineis
		 * @return Valor flutuante para multiplicacao
		 */
		float proporcao() const;
		void atualizar();
		std::shared_ptr<Bubble::Inputs::Inputs> inputs{ nullptr };
		std::shared_ptr<Contexto> contexto{ nullptr };
	private:
		std::shared_ptr<Colisao2d> colisao{ nullptr };
		Ancora* pai { nullptr };
		TipoAncoragem lado{ Horizontal };
		bool mouse_1click, arrastando;
		Vector2 arrasto_pos, mouse_pos_ini;
		float _Mproporcao = 0.5F;
		void atualizarColisao();
		void atualizarCursor();
		void atualizarArrasto();
	};
}