
/** @copyright Copyright (c) 2024 Daniel Oliveira */

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
			Rect(std::shared_ptr<Contexto> ctx, const Vector4 &rect);
			virtual Vector4 obterRetangulo() const;
			Vector4f obtRectNDC() const;
			virtual void definirTamanho(const Vector2 &tam);
			virtual void definirPosicao(const Vector2 &pos);
			void adiTam(const Vector2 &tam);
			void adiPos(const Vector2 &tam);
			virtual void definirRetangulo(const Vector4 &rect);
			virtual void defCor(const Color &cor);
			void defCor(Color* cor);
			virtual void atualizar();
			virtual void renderizar();
			void definirBuffers(std::shared_ptr<Contexto> ctx, const Vector4& rect = {});
		protected:
			std::shared_ptr<Contexto> contexto{ nullptr };
			Vector4 retangulo{30,30,30,30}, tamanhoAnterior;
			Color* cor_base	{ new Color(0.2f, 0.2f, 0.2f, 1.f) };
			Vector4f paraNDC();
			Vector4f coord_ndc;
			Size tamanhoDoContextoAnterior;
			Shader shader{ Shader("assets/shaders/quad.vert", "assets/shaders/quad.frag") };
		};
	}
}
