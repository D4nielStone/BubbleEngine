#pragma once
#include "rect.hpp"
#include "meio_circulo.hpp"
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
			void definirTamanho(const Vector2& tam)	override;
			void definirPosicao(const Vector2& pos)	override;
			void definirRetangulo(const Vector4& rect) override;
			Vector4 obterRetangulo() const override;
			std::shared_ptr<Contexto> obterContexto() const;
			Vector2 posicaoWidget{ 0, 0 };
			Vector2 widgetPadding{ 3, 3 };
			Color arvoreCor{ 0.1f, 0.1f, 0.1f };
			bool selecionado{ true };
			bool ocultar_linhas{ false };
		protected:
			virtual void preAtualizacao() {};
			virtual void posAtualizacao() {};
			// Define a espessura da borda
			int espessuraBorda{ 8 };
			void configurar(std::shared_ptr<BubbleUI::Contexto> contexto, const Vector4& retangulo);
			Vector4 retangulo_completo{};
			std::unique_ptr<Rect>
				borda_d{ nullptr },
				borda_e{ nullptr },
				borda_c{ nullptr }, 
				borda_b{ nullptr };
			std::unique_ptr<MeioCirculo>
				ponta_a{ nullptr },
				ponta_b{ nullptr },
				ponta_c{ nullptr }, 
				ponta_d{ nullptr };
		};
	}
}