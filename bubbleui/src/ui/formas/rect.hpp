#pragma once
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
			void defCor(Color cor);
			void atualizar(float deltaTime);
			void renderizar();
		protected:
			Contexto* contexto;
			Vector4 retangulo;
			Color cor_base;
		private:
			Vector4f paraNDC();
			void definirBuffers();
			Vertex vertex;
		};
	}
}
