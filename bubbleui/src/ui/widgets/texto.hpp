#pragma once
#include "string"
#include "src/ui/widgets/widget.hpp"
#include "src/ui/formas/rect.hpp"
#include "src/arquivadores/fonte.hpp"


namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API Texto : public Widget
		{
		public:
			Texto(std::string* label);
			Texto(std::string* label, unsigned int resolucao);
			Texto(std::string label);
			Texto(std::string label, unsigned int resolucao);
			~Texto();
			void atualizar(float deltaTime) override;
			void renderizar() override;
			void defPainel(Painel*) override;
		private:
			void configurar(unsigned int resolucao = 16, std::string font_path = "assets/fontes/noto-sans/notosans-bold.ttf");
			Formas::Rect* corpo_do_texto = nullptr;
			unsigned int resolucao, texturaID;
			std::string* label;
			Vector2 pos_texto;
		};
	}
}