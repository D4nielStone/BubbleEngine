#pragma once
#include "glad/glad.h"
#include "bubbleui.hpp"
#include "src/util/utils.hpp"
#include "src/ui/contexto/contexto.hpp"

namespace BubbleUI
{
	namespace Formas
	{

		class BEUI_DLL_API Linha
		{
		public:
			Linha() = default;
			Linha(const Vector4<int> &posicoes, std::shared_ptr<Contexto> ctx);
			void definirPosicao(const Vector4<int> &posicoes);
			void defCor(const Color &cor);
			void definirCorRef(Color* cor);
			void atualizar();
			void renderizar();
		protected:
			std::shared_ptr<Contexto> contexto;
			Vector4<int> posicoes, tamanhoAnterior;
			Color cor_base;
			Color* cor_referencial{ nullptr };
		private:	
			Vector4<float> paraNDC();
			Vector4<float> coord_ndc;
			Size tamanhoDoContextoAnterior;
			Bubble::Arquivadores::Shader shader = Bubble::Arquivadores::Shader("assets/shaders/linha.vert", "assets/shaders/linha.frag");
		};
	}
}
