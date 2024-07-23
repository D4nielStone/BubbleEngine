#pragma once
#include "src/util/includes.h"
#include "src/inputs/inputs.h"
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
			virtual void renderizar();
			virtual void atualizar();
			void defPos(Vector2 pos);
			void defTam(Vector2 tam);
			void defCor(Color cor);
			Vector2 obtPos() const;
			Vector2 obtTam() const;
			Color obtCor() const;
			Quadrado* obtPai();
			void defPai(Quadrado* pai);
			void defInputs(Inputs::Inputs* inp);
			bool arrastando();
			int tamanho_ui = 1;
			Vector2 ArrastoPos;
			Vector2 inicioArrastoPos;
			Vector2 inicioMouseArrasto;
		protected:
			float aspect = 0;
			unsigned int VAO;
			Quadrado* pai = nullptr;
			glm::mat4 projecao;
			Shader shader;
			Vector2 posicaoG, tamanho, posicao;
			Color cor;
			GLFWwindow* janelaglfw;
			Vector2 janelaTam;
			// click
			void converterMouse(Vector2* mousepos);
			bool mouseEmCima(Vector2 mouse);
			bool mouseArrastando;
			Inputs::Inputs* inputs;
		private:
			bool once;
			unsigned int VBO, EBO;
			void definirBuffer();
		};
	}
}
