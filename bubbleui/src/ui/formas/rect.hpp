#pragma once
#include "glad/glad.h"
#include "bubbleui.hpp"
#include "src/util/includes.hpp"
#include "src/ui/contexto/contexto.hpp"

namespace BubbleUI
{
	namespace Formas
	{

		class BEUI_DLL_API Rect
		{
		public:
			Rect() = default;
			Rect(Contexto* ctx);
			Rect(Vector4 rect, Contexto* ctx);
			Vector4 obtRect() const;
			void defTam(Vector2f tam);
			void defPos(Vector2f pos);
			void defCor(Color cor);
			void atualizar(float deltaTime);
			void renderizar(GLenum modo);
		protected:
			Contexto* contexto;
			Vector4 retangulo;
			Color cor_base;
		private:
			Vector4f paraNDC();
			Vector4f coord_ndc;
			void definirBuffers();
			Vertex vertex;
		};
	}
}
