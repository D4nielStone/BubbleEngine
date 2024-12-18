#pragma once
#include "glad/glad.h"
#include "bubbleui.hpp"
#include "src/util/cor.hpp"
#include "src/util/vetor2.hpp"
#include "src/ui/contexto/contexto.hpp"

namespace BubbleUI
{
	namespace Formas
	{

		class BEUI_DLL_API Linha
		{
		public:
			Linha() = default;
			Linha(const bubble::vetor4<int> &posicoes, std::shared_ptr<Contexto> ctx);
			void definirPosicao(const bubble::vetor4<int> &posicoes);
			void defCor(const bubble::cor &cor);
			void definirCorRef(bubble::cor* cor);
			void atualizar();
			void renderizar();
		protected:
			std::shared_ptr<Contexto> contexto;
			bubble::vetor4<int> posicoes, tamanhoAnterior;
			bubble::cor cor_base;
			bubble::cor* cor_referencial{ nullptr };
		private:	
			bubble::vetor4<float> paraNDC();
			bubble::vetor4<float> coord_ndc;
			bubble::vetor2<int> tamanhoDoContextoAnterior;
			Bubble::Arquivadores::Shader shader = Bubble::Arquivadores::Shader("assets/shaders/linha.vert", "assets/shaders/linha.frag");
		};
	}
}
