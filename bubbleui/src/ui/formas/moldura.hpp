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
			void atualizar() override;
			void renderizar(GLenum) override;
			Contexto* obtCtx() const;
			Vector2f widget_pos;
			Vector2 widget_padding{ 3, 3 };
			bool ocultar_linhas{ false };
		protected:
			virtual void preAtualizacao() {};
			virtual void posAtualizacao() {};
			std::unique_ptr<Linha>
				linha_d{ nullptr },
				linha_e{ nullptr },
				linha_c{ nullptr }, 
				linha_b{ nullptr };
		};
	}
}