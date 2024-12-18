#pragma once
#include "imagem.hpp"
#include "texto.hpp"

namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API SeletorDeCor : public Texto
		{
		public:
			SeletorDeCor(bubble::cor* cor, const std::string& label, const Alinhamento& alinhamento = Alinhamento::Centro);
			void atualizar() override;
			void renderizar() override;
			void definirPai(Formas::Moldura* painel) override;
			void calcularCor();
		private:
			bubble::cor* cor_callback{ nullptr };
			const unsigned int raio{ 48 };
			float angulo{}, hue{}, saturacao{}, brilho{0.5};
			std::unique_ptr<Imagem> arco_cor{ nullptr };
		};
	}
}