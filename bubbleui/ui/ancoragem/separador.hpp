#pragma once
#include "ancora.hpp"
#include "src/ui/formas/colisao2d.hpp"
#include "src/util/vetor2.hpp"

namespace bubbleui
{
	struct separador
	{
		/**
		 * @brief Construtor do separador
		 * @param tipo Tipo do separador (Vertical ou Horizontal)
		 * @param ancora Ancora pai para redimesionamento
		 */
		separador(tipoAncoragem tipo, ancora* ancora);

		/**
		 * @brief Proporcao de divisao entre paineis
		 * @return Valor flutuante para multiplicacao
		 */
		float proporcao() const;
		void atualizar();
		std::shared_ptr<Bubble::Inputs::Inputs> inputs{ nullptr };
		std::shared_ptr<contexto> contexto{ nullptr };

		std::shared_ptr<Colisao2d> colisao{ nullptr };
		ancora* pai { nullptr };
		tipoAncoragem lado{ Horizontal };
		bool mouse_1click, arrastando;
		bubble::vetor2<int> arrasto_pos, mouse_pos_ini;
		float _Mproporcao = 0.5F;
		void atualizarColisao();
		void atualizarCursor();
		void atualizarArrasto();
	};
}