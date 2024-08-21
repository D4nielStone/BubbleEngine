#pragma once
#include "glad/glad.h"
#include "bubbleui.hpp"
#include "src/util/includes.hpp"
#include "src/ui/contexto/contexto.hpp"

namespace BubbleUI
{
	namespace Formas
	{

		class BEUI_DLL_API Rect
		{
		public:
			Rect() = default;
			Rect(Contexto* ctx);
			Rect(Vector4 rect, Contexto* ctx);
			Vector4 obtRect() const;
			void defTam(Vector2f tam);
			void defPos(Vector2f pos);
			void adiTam(Vector2 tam);
			void adiPos(Vector2 tam);
			void defCor(Color cor);
			virtual void atualizar(float deltaTime);
			virtual void renderizar(GLenum modo);
		protected:
			Contexto* contexto{ nullptr };
			Vector4 retangulo;
			Color cor_base{ 0.2, 0.2, 0.2 };
			Vector4f paraNDC();
			Vector4f coord_ndc;
			Shader shader{ Shader("assets/shaders/quad.vert", "assets/shaders/quad.frag") };
			void definirBuffers();
		};
	}
}
