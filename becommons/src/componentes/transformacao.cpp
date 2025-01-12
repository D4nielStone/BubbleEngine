#include "transformacao.hpp"
#include <src/nucleo/fase.hpp>

void bubble::transformacao::apontarEntidade(const uint32_t& ent)
{
	if (!fase_atual->obterRegistro()->tem<bubble::transformacao>(ent))return;
	alvo = &fase_atual->obterRegistro()->obter<bubble::transformacao>(ent)->posicao;
}

void bubble::transformacao::apontarV3(const glm::vec3& pos)
{
	*alvo = pos;
}