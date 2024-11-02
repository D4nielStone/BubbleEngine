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
		class BEUI_DLL_API Texto
		{
		public:
			Texto(std::string* label_shared);
			Texto(const std::string &label_shared);
			Texto() = default;
			~Texto();
			virtual void atualizar();
			virtual void renderizar() const;
			static void configurar(const std::string &font_path = "assets/fontes/consolas/consolas.ttf", unsigned int resolucao = 12);
			virtual void definirPai(Formas::Moldura*);
			void definirEscondido(bool boleano);
			void quebrarLinha(bool boleano);
			int largura{ 0 }, altura{ 0 };
		protected:
			Vector2f box_pos{ 0, 0 }, line_pos{ 0, 0 };
			void renderizar_texto();
			unsigned int resolucao, texturaID;
			int lines_box_limite, largura_texto;
			Vector2 box_size{ 0, 0 }, letra_padding{2, 2}, pos_texto{ 0, 0 };
			std::string texto{ "" }, frase{ "" };
			std::string* label_shared{ nullptr };
			Color cor{ 1, 1, 1 };
			Vector4 char_rect{ 0, 0, 0 ,0 };
			Vector4f paraNDC();
			Shader shader = Shader("assets/shaders/texto.vert", "assets/shaders/texto.frag");
			std::shared_ptr<Contexto> contexto{ nullptr };
			Formas::Moldura* pai{ nullptr };
			std::vector<LetraRect> letras_rect;
			bool vquebrarLinha{ true }, escondido{ false };
		};
	}
}