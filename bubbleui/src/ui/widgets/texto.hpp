#pragma once
#include "string"
#include "src/ui/widgets/widget.hpp"
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
			Texto() = default;
			~Texto();
			void atualizar(float deltaTime) override;
			void renderizar() override;
			static void configurar(unsigned int resolucao = 12, std::string font_path = "assets/fontes/noto-sans/notosans-bold.ttf");
		protected:
			void renderizar_texto();
			unsigned int resolucao, texturaID;
			int lines_box_limite;
			Vector2f box_size, box_pos, line_pos;
			Vector2 letra_padding;
			std::string texto, frase;
			Color cor;
			Vector2 pos_texto;
		private:
			std::string* label;

		};
	}
}