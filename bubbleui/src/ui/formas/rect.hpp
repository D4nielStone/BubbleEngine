
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#pragma once
#include "glad/glad.h"
#include "bubbleui.hpp"
#include "src/util/utils.hpp"
#include "src/ui/contexto/contexto.hpp"

namespace BubbleUI
{
	namespace Formas
	{

		class BEUI_DLL_API Rect
		{
		public:
			Rect() = default;
			Rect(std::shared_ptr<Contexto> ctx, const Vector4<int> &rect);
			virtual Vector4<int> obterRetangulo() const;
			Vector4<float> obtRectNDC() const;
			virtual void definirTamanho(const Vector2 &tam);
			virtual void definirPosicao(const Vector2 &pos);
			void adiTam(const Vector2 &tam);
			void adiPos(const Vector2 &tam);
			virtual void definirRetangulo(const Vector4<int> &rect);
			virtual void defCor(const Color &cor);
			void defCor(Color* cor);
			virtual void atualizar();
			virtual void renderizar();
			void definirBuffers(std::shared_ptr<Contexto> ctx, const Vector4<int>& rect = {});
		protected:
			std::shared_ptr<Contexto> contexto{ nullptr };
			Vector4<int> retangulo{30,30,30,30}, tamanhoAnterior;
			Color* cor_base	{ new Color(ROXO_ESCURO) };
			Vector4<float> paraNDC();
			Vector4<float> coord_ndc;
			Size tamanhoDoContextoAnterior;
			Bubble::Arquivadores::Shader shader{ Bubble::Arquivadores::Shader("assets/shaders/quad.vert", "assets/shaders/quad.frag") };
		};
	}
}
