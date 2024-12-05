
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

		class BEUI_DLL_API MeioCirculo
		{
		public:
			MeioCirculo() = default;
			MeioCirculo(std::shared_ptr<Contexto> ctx, const Vector4<int> &rect);
			virtual Vector4<int> obtMeioCirculo() const;
			Vector4<float> obtMeioCirculoNDC() const;
			void definirTamanho(const Vector2 &tam);
			void definirPosicao(const Vector2 &pos);
			void adiTam(const Vector2 &tam);
			void adiPos(const Vector2 &tam);
			void definirRetangulo(const Vector4<int> &rect);
			void defCor(const Color &cor);
			void defCor(Color* cor);
			virtual void atualizar();
			virtual void renderizar() ;
		protected:

			std::shared_ptr<Contexto> contexto{ nullptr };
			Vector4<int> retangulo, tamanhoAnterior;
			Color* cor_base	{ new Color(0.2f, 0.2f, 0.2f, 1.f) };
			Vector4<float> paraNDC();
			Vector4<float> coord_ndc;
			Size tamanhoDoContextoAnterior;
			Bubble::Arquivadores::Shader shader{ Bubble::Arquivadores::Shader("assets/shaders/quad.vert", "assets/shaders/quad.frag") };
			void definirBuffers();
		};
	}
}
