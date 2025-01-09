#include "api_entidade.hpp"
#include <src/nucleo/fase.hpp>
#include <src/componentes/renderizador.hpp>
#include <src/componentes/codigo.hpp>

using namespace bubble;

void bapi::entidade::destruir() const
{
	auto componentes = fase_atual->obterRegistro()->obterComponentes(id);
	if (componentes & bubble::componente::COMPONENTE_CAM)
		fase_atual->obterRegistro()->remover<camera>(id);
	if (componentes & bubble::componente::COMPONENTE_RENDER)
		fase_atual->obterRegistro()->remover<renderizador>(id);
	if (componentes & bubble::componente::COMPONENTE_TRANSFORMACAO)
		fase_atual->obterRegistro()->remover<transformacao>(id);
	if (componentes & bubble::componente::COMPONENTE_CODIGO)
		fase_atual->obterRegistro()->remover<codigo>(id);
	if (componentes & bubble::componente::COMPONENTE_FISICA)
		fase_atual->obterRegistro()->remover<fisica>(id);
	if (componentes & bubble::componente::COMPONENTE_TEXTO)
		fase_atual->obterRegistro()->remover<texto>(id);
	if (componentes & bubble::componente::COMPONENTE_IMAGEM)
		fase_atual->obterRegistro()->remover<imagem>(id);

}

bapi::entidade::entidade(const uint32_t& id) : id(id)
{
	if (fase_atual->obterRegistro()->tem<bubble::transformacao>(id))
		_Mtransformacao = fase_atual->obterRegistro()->obter<bubble::transformacao>(id).get();
	if (fase_atual->obterRegistro()->tem<bubble::camera>(id))
		_Mcamera = fase_atual->obterRegistro()->obter<bubble::camera>(id).get();
	if (fase_atual->obterRegistro()->tem<bubble::texto>(id))
		_Mtexto = fase_atual->obterRegistro()->obter<bubble::texto>(id).get();
	if (fase_atual->obterRegistro()->tem<bubble::imagem>(id))
		_Mimagem = fase_atual->obterRegistro()->obter<bubble::imagem>(id).get();
	if (fase_atual->obterRegistro()->tem<bubble::fisica>(id))
		_Mfisica = fase_atual->obterRegistro()->obter<bubble::fisica>(id).get();
}