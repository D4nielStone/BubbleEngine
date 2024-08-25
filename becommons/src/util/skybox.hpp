#pragma once
#include "src/util/includes.hpp"
#include "becommons.hpp"
#include "src/arquivadores/shader.hpp"
namespace Bubble
{
	namespace Util
	{
		class BECOMMONS_DLL_API Skybox
		{
		private:
			Vertex* mVertex;
			const char* Path = nullptr;
			unsigned int cubemapTexture, VBO, VAO, EBO;
		public:
			Shader shader;
			Skybox(const char* path);
			Skybox();
			void configurarBuffers();
			void renderizar();
			unsigned int loadCubemapFromSingleTexture(const char* path);
		};
	}
}