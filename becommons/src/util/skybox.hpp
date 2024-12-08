#pragma once
#include "src/util/utils.hpp"
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
			Malha malha;
			const char* Path = nullptr;
		public:
			Bubble::Arquivadores::Shader shader = Bubble::Arquivadores::Shader("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
			Skybox(const char* path);
			Skybox();
			void configurarBuffers();
			void renderizar(const glm::mat4& projectionMat, const glm::mat4& viewMat);
			unsigned int loadCubemapFromSingleTexture(const char* path);
		};
	}
}