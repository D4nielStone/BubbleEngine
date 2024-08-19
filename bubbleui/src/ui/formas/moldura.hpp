#pragma once
#include "rect.hpp"
#include "linha.hpp"
namespace BubbleUI
{
	namespace Formas
	{
		class BEUI_DLL_API Moldura : public Rect
		{
		public: 
			Moldura(Contexto*, Vector4 retangulo = {0, 0, 100, 20});
			Moldura() = default;
			void atualizar(float deltaTime) override;
			void renderizar(GLenum) override;
		private:
			Linha* linha_d,* linha_e,* linha_c,* linha_b;
		};
	}
}