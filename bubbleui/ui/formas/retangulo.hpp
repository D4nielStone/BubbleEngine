/** @copyright Copyright (c) 2024 Daniel Oliveira */

#pragma once
#include "src/util/vetor4.hpp"
#include "src/util/vetor2.hpp",
#include "src/arquivadores/shader.hpp"

namespace bubbleui
{
		struct retangulo
		{
			retangulo() = default;
			retangulo(const bubble::vetor4<double> &rect);
			virtual void atualizar() {};

			bubble::vetor4<double> bounds{0,0,60,60};
			bubble::cor cor_base;
		};
} /// namespace bubbleui
