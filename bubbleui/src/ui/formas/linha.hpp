#pragma once
#include "glad/glad.h"
#include "bubbleui.hpp"
#include "src/util/includes.hpp"
#include "src/ui/contexto/contexto.hpp"

namespace BubbleUI
{
	namespace Formas
	{

		class BEUI_DLL_API Linha
		{
		public:
			Linha() = default;
			Linha(const Vector4 &posicoes, std::shared_ptr<Contexto> ctx);
			void defPos(const Vector4 &posicoes);
			void defCor(const Color &cor);
			void atualizar();
			void renderizar() const;
		protected:
			std::shared_ptr<Contexto> contexto;
			Vector4 posicoes, tamanhoAnterior;
			Color cor_base;
		private:	
			Vector4f paraNDC();
			Vector4f coord_ndc;
			Size tamanhoDoContextoAnterior;
			Shader shader = Shader("assets/shaders/linha.vert", "assets/shaders/linha.frag");
		};
	}
}
