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
			Texto(std::string* label_shared);
			Texto(const std::string& label_shared);
			Texto() = default;
			void definirTexto(const std::string & texto);
			void atualizar() override;
			void renderizar() const override;
			static void configurar(unsigned int resolucao = 12, std::string font_path = "assets/fontes/consolas/consolas.ttf");
		protected:
			bool desenharSelecao(Vector2 mouse_inicial, Vector2 mouse_final, Vector4 char_rect, size_t letra_idx);
			void renderizar_texto(std::string &frase);
			size_t texto_cursor_index{ 0 }; Vector4f texto_cursor_pos;
			bool selecionando_texto{ false };
			unsigned int resolucao, texturaID;
			int lines_box_limite, largura_texto;
			Vector2f box_size, box_pos, line_pos;
			Vector2 letra_padding{ 5, 5 };
			std::string frase;
			Color cor{1.f, 1.f, 1.f, 1.f}, cor_de_selecao{ 0.678, 0.847, 0.902, 1.0 };
			Vector2 pos_texto;
			Vector4 char_rect, char_fundo_rect;
			Vector4f paraNDC(const Vector4& vec4);
			std::vector<LetraRect> letras_rect;
			Vector4 area_de_selecao;
			Shader shader = Shader("assets/shaders/texto.vert", "assets/shaders/texto.frag"), shaderQuad = Shader("assets/shaders/quad.vert", "assets/shaders/quad.frag");
			std::string* label_shared{ nullptr };
		};
	}
}