
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#pragma once
#include "glad/glad.h"
#include "bubbleui.hpp"
#include "src/util/vetor4.hpp"
#include "src/util/vetor2.hpp"
#include "src/ui/contexto/contexto.hpp"

namespace BubbleUI
{
	namespace Formas
	{

		class BEUI_DLL_API MeioCirculo
		{
		public:
			MeioCirculo() = default;
			MeioCirculo(std::shared_ptr<Contexto> ctx, const bubble::vetor4<int> &rect);
			virtual bubble::vetor4<int> obtMeioCirculo() const;
			bubble::vetor4<float> obtMeioCirculoNDC() const;
			void definirTamanho(const Vector2 &tam);
			void definirPosicao(const Vector2 &pos);
			void adiTam(const Vector2 &tam);
			void adiPos(const Vector2 &tam);
			void definirRetangulo(const bubble::vetor4<int> &rect);
			void defCor(const bubble::cor &cor);
			void defCor(bubble::cor* cor);
			virtual void atualizar();
			virtual void renderizar() ;
		protected:

			std::shared_ptr<Contexto> contexto{ nullptr };
			bubble::vetor4<int> retangulo, tamanhoAnterior;
			bubble::cor* cor_base	{ new bubble::cor(0.2f, 0.2f, 0.2f, 1.f) };
			bubble::vetor4<float> paraNDC();
			bubble::vetor4<float> coord_ndc;
			bubble::vetor2<int> tamanhoDoContextoAnterior;
			Bubble::Arquivadores::Shader shader{ Bubble::Arquivadores::Shader("assets/shaders/quad.vert", "assets/shaders/quad.frag") };
			void definirBuffers();
		};
	}
}
