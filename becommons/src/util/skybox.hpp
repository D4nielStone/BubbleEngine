#pragma once
#include "src/util/includes.hpp"
#include "becommons.hpp"
#include "src/arquivadores/shader.hpp"
namespace Bubble
{
	namespace Util
	{
		BECOMMONS_DLL_API unsigned int obterSkyboxTexture();
		class BECOMMONS_DLL_API Skybox
		{
		private:
			Vertex malha;
			const char* Path = nullptr;
		public:
			Shader shader = Shader("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
			Skybox(const char* path);
			Skybox();
			void configurarBuffers();
			void renderizar() const;
			unsigned int loadCubemapFromSingleTexture(const char* path);
		};
	}
}