#include "api_entidade.hpp"
#include <src/nucleo/cena.hpp>

bapi::entidade::entidade(const uint32_t& id) : id(id)
{
	if (cena_atual->obterRegistro()->tem<bubble::transformacao>(id))
		_Mtransformacao = cena_atual->obterRegistro()->obter<bubble::transformacao>(id).get();
	if (cena_atual->obterRegistro()->tem<bubble::camera>(id))
		_Mcamera = cena_atual->obterRegistro()->obter<bubble::camera>(id).get();
}