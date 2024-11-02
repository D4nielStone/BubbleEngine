#pragma once
#include "imagem.hpp"
#include "texto.hpp"
#include "src/ui/painel/separador.hpp"

namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API SeletorDeCor : public Texto
		{
		public:
			SeletorDeCor(Color* cor, const std::string& label, const Alinhamento& alinhamento = Alinhamento::Centro);
			void atualizar() override;
			void renderizar() const override;
			void definirPai(Formas::Moldura* painel) override;
			void calcularCor();
		private:
			Color* cor_callback{ nullptr };
			const unsigned int raio{ 48 };
			float angulo{}, hue{}, saturacao{}, brilho{0.5};
			std::unique_ptr<Imagem> arco_cor{ nullptr };
		};
	}
}