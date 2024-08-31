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
			void atualizar() override;
			void renderizar() override;
			static void configurar(unsigned int resolucao = 12, std::string font_path = "assets/fontes/arial/arialbd.ttf");
		protected:
			void renderizar_texto();
			unsigned int resolucao, texturaID;
			int lines_box_limite, largura_texto;
			Vector2f box_size, box_pos, line_pos;
			Vector2 letra_padding;
			std::string frase;
			Color cor, cor_de_selecao;
			bool letra_selecionada{ false };
			Vector2 pos_texto;
			Vector4 char_rect, char_fundo_rect;
			Vector4f paraNDC(Vector4);
			std::vector<LetraRect> letras_rect;
			Vector4 area_de_selecao;
			Shader shader = Shader("assets/shaders/texto.vert", "assets/shaders/texto.frag"), shaderQuad = Shader("assets/shaders/quad.vert", "assets/shaders/quad.frag");
			std::string* label{ nullptr };
		};
	}
}