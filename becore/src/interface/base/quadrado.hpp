#ifndef QUADRADO_HPP
#define QUADRADP_HPP
#include "src/util/includes.h"
#include "src/arquivadores/shader.h"
#include "becore.h"
#include "glm/glm.hpp"

class GLFWwindow;

namespace Bubble
{
	namespace Interface
	{

		class BECORE_DLL_API Quadrado
		{
		public:
			Quadrado();
			Quadrado(Vector2 posicao, Vector2 tamanho);
			void defPos(Vector2 pos);
			void defTam(Vector2 tam);
			void defCor(Color cor);
			Vector2 obtPos() const { return posicaoG; };
			Vector2 obtTam() const { return tamanho; };
			Color obtCor() const { return cor; };
			void definirBuffer();
			virtual void renderizar();
			virtual void atualizar();
			Quadrado* pai = nullptr;
		protected:
			glm::mat4 projecao;
			Shader shader;
			Vector2 posicaoG, tamanho, posicao;
			Color cor;
			unsigned int VBO, VAO, EBO;
			GLFWwindow* janelaglfw;
		};
	}
}
#endif // !QUADRADO_HPP
