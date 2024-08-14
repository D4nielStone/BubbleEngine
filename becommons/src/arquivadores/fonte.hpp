#pragma once
#include "becommons.hpp"
#include "glm/glm.hpp"
#include "string"
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Bubble
{
	namespace Arquivadores
	{
		struct BECOMMONS_DLL_API Character
		{
			unsigned int TextureID;  // ID handle of the glyph texture
			glm::ivec2   Size;       // Size of glyph
			glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
			unsigned int Advance;    // Offset to advance to next glyph
		};
		BECOMMONS_DLL_API std::map<char, Character>* obterCaracteres();
		BECOMMONS_DLL_API void carregarFonte(std::string path);
		BECOMMONS_DLL_API void definirResolucao(int resolution);
	}
}