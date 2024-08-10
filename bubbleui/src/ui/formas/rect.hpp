#pragma once
#include "bubbleui.h"
#include "src/util/includes.h"
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
			virtual void atualizar(float deltaTime);
			virtual void renderizar();
		protected:
			Contexto* contexto;
			Vector4 retangulo;
		private:
			void definirBuffers();
			Vertex vertex;
		};
	}
}
