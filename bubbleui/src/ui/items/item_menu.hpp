#pragma once
#include "bubbleui.hpp"
#include "string"
#include "src/arquivadores/fonte.hpp"
#include "src/ui/formas/moldura.hpp"
#include "src/ui/formas/colisao2d.hpp"
namespace BubbleUI
{
	namespace Items
	{
		struct LetraRect
		{
			Vector4f rect;
			unsigned int ID;
		};
		class BEUI_DLL_API ItemMenu
		{
		public:
			ItemMenu(std::string* label);
			ItemMenu(std::string* label, unsigned int resolucao);
			ItemMenu(std::string label);
			ItemMenu(std::string label, unsigned int resolucao);
			ItemMenu() = default;
			~ItemMenu();
			virtual void atualizar();
			virtual void renderizar();
			static void configurar(unsigned int resolucao = 12, std::string font_path = "assets/fontes/arial/arialbd.ttf");
			virtual void defMoldura(Formas::Moldura*);
			int largura{ 0 }, altura{ 0 };
		protected:
			void renderizar_texto();
			unsigned int resolucao, texturaID;
			int lines_box_limite, largura_texto;
			Vector2f box_size{ 0, 0 }, box_pos{ 0, 0 }, line_pos{ 0, 0 };
			Vector2 letra_padding{2, 2};
			std::string texto{ "" }, frase{ "" };
			Color cor{ 1, 1, 1 };
			Vector2 pos_texto{ 0, 0 };
			Vector4 char_rect{ 0, 0, 0 ,0 };
			Vector4f paraNDC();
			Shader shader = Shader("assets/shaders/texto.vert", "assets/shaders/texto.frag");
			Colisao2d* colisao{ nullptr };
			Formas::Moldura moldura, * pai{ nullptr };
			std::vector<LetraRect> letras_rect;
			bool gatilho{ true };
			bool clicado{ false }, mouseEmCima{ false };
			std::string* label;
		};
	}
}