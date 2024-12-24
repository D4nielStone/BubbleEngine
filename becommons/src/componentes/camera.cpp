#include "camera.hpp"
#include "transformacao.hpp"
#include <src/nucleo/cena.hpp>

void bubble::camera::olhar(const uint32_t& ent)
{
	if(cena_atual->obterRegistro()->tem<bubble::transformacao>(ent))
	alvo = &cena_atual->obterRegistro()->obter<bubble::transformacao>(ent)->posicao;
}