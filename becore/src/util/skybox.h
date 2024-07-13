#ifndef SKYBOX_H
#define SKYBOX_H
#include "includes.h"
#include "becore.h"
#include "src/arquivadores/shader.h"
namespace Bubble
{
	namespace Util
	{
		class BECORE_DLL_API Skybox
		{
		private:
			Vertex mVertex;
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

#endif