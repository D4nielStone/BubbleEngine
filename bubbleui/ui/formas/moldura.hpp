#pragma once
#include "rect.hpp"
#include "meio_circulo.hpp"
#include "linha.hpp"

namespace BubbleUI
{
	namespace Formas
	{
		class Moldura : public Rect
		{
		public: 
			Moldura(std::shared_ptr<Contexto>, const bubble::vetor4<int>& retangulo = {0, 0, 100, 20});
			Moldura() = default;
			void atualizar() override;
			void renderizar() override;
			void definirTamanho(const Vector2& tam)	override;
			void definirPosicao(const Vector2& pos)	override;
			void definirRetangulo(const bubble::vetor4<int>& rect) override;
			bubble::vetor4<int> obterRetangulo() const override;
			std::shared_ptr<Contexto> obterContexto() const;
			Vector2 posicaoWidget{ 0, 0 };
			Vector2 widgetPadding{ 3, 3 };
			bubble::cor arvoreCor{ 0.1f, 0.1f, 0.1f };
			bool selecionado{ true };
			bool ocultar_linhas{ false };
		protected:
			virtual void preAtualizacao() {};
			virtual void posAtualizacao() {};
			// Define a espessura da borda
			int espessuraBorda{ 3 };
			void configurar(std::shared_ptr<BubbleUI::Contexto> contexto, const bubble::vetor4<int>& retangulo);
			bubble::vetor4<int> retangulo_completo{};
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