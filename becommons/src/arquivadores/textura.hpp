#pragma once
#include "imageloader.hpp"
#include "becommons.hpp"

namespace Bubble
{
	namespace Arquivadores
	{
		class BECOMMONS_DLL_API Textura
		{
		public:
			Textura(ImageLoader* img);
			void alterarBitMap(unsigned char* data);
			void use();
		private:
			ImageLoader* imagem;
			void configurarBuffers();
			unsigned int ID;
		};
	}
}