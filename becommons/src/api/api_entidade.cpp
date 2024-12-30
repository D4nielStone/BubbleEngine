#include "api_entidade.hpp"
#include <src/nucleo/fase.hpp>

bapi::entidade::entidade(const uint32_t& id) : id(id)
{
	if (fase_atual->obterRegistro()->tem<bubble::transformacao>(id))
		_Mtransformacao = fase_atual->obterRegistro()->obter<bubble::transformacao>(id).get();
	if (fase_atual->obterRegistro()->tem<bubble::camera>(id))
		_Mcamera = fase_atual->obterRegistro()->obter<bubble::camera>(id).get();
}