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
			Moldura(std::shared_ptr<Contexto>, const Vector4& retangulo = {0, 0, 100, 20});
			Moldura() = default;
			void atualizar() override;
			void renderizar() const override;
			std::shared_ptr<Contexto> obtCtx() const;
			Vector2f posicaoWidget{ 0, 0 };
			Vector2 widgetPadding{ 3, 3 };
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