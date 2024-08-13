#pragma once
#include "src/ui/widgets/widget.hpp"
#include "string"
#include "src/ui/formas/rect.hpp"
#include <ft2build.h>
#include "glm/glm.hpp"
#include <map>

#include FT_FREETYPE_H
namespace BubbleUI
{
	namespace Widgets
	{
		struct Character {
			unsigned int TextureID;  // ID handle of the glyph texture
			glm::ivec2   Size;       // Size of glyph
			glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
			unsigned int Advance;    // Offset to advance to next glyph
		};

		std::map<char, Character> Characters;

		class BEUI_DLL_API Texto : public Widget
		{
		public:
			Texto(std::string label);
			Texto(std::string label, unsigned int pxl);
			~Texto();
			void atualizar(float deltaTime) override;
			void renderizar() override;
		private:
			FT_Face face;
			FT_Library library;
			Formas::Rect* corpo_do_texto = nullptr;
			unsigned int resolucao, texturaID;
			std::string label;
		};
	}
}