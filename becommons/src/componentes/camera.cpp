#include "camera.hpp"
#include "transformacao.hpp"
#include <src/nucleo/fase.hpp>
#include <os/janela.hpp>

void bubble::camera::olhar(const uint32_t& ent)
{
	if(fase_atual->obterRegistro()->tem<bubble::transformacao>(ent))
	alvo = &fase_atual->obterRegistro()->obter<bubble::transformacao>(ent)->posicao;
}
void bubble::camera::olharPara(const glm::vec3& pos)
{
	*alvo = pos;
}

bubble::camera::camera(const vetor3<float>& pos, const flagCamera& flags)
	: posicao({ pos.x,pos.y,pos.z }), flags(flags)
{
}