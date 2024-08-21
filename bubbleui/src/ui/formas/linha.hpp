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
			Linha(Vector4f posicoes, Contexto* ctx);
			Linha() = default;
			void defPos(Vector4f posicoes);
			void defCor(Color cor);
			void atualizar(float deltaTime);
			void renderizar();
		protected:
			Contexto* contexto;
			Vector4f posicoes;
			Color cor_base;
		private:
			Vector4f paraNDC();
			Vector4f coord_ndc;
			Shader shader = Shader("assets/shaders/linha.vert", "assets/shaders/linha.frag");
		};
	}
}
