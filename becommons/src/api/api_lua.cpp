#include "api_lua.hpp"
#include "../nucleo/fase.hpp"
#include "../componentes/renderizador.hpp"
#include "../nucleo/sistema_de_fisica.hpp"
#include "../componentes/codigo.hpp"

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
void bapi::definirFisica(lua_State* L)
{
	luabridge::getGlobalNamespace(L).
		beginClass<bubble::fase>("fase").		///< define vetor3
		addConstructor<void(*)(const char*)>().
		addFunction("pausar", &bubble::fase::pausar).
		addFunction("parar", &bubble::fase::parar).
		endClass().
		beginClass<btCollisionObject>("objetoDeColisao").			///< define transformacao
		addConstructor<void(*)()>().
		endClass().
		beginClass<btRigidBody>("corpoRigido").			///< define transformacao
		endClass().
		beginClass<bubble::raio>("raio").			///< define transformacao
		addConstructor<void(*)()>().
		addData("origem", &bubble::raio::origem).
		addData("direcao", &bubble::raio::direcao).
		endClass().
		beginClass<bubble::resultadoRaio>("resultadoRaio").			///< define transformacao
		addConstructor<void(*)()>().
		addData("objetoAtingido", &bubble::resultadoRaio::objetoAtingido, false).
		addData("atingiu", &bubble::resultadoRaio::atingiu, false).
		addData("pontoDeColisao", &bubble::resultadoRaio::pontoDeColisao, false).
		addData("normalAtingida", &bubble::resultadoRaio::normalAtingida, false).
		endClass().
		beginClass<bubble::fisica>("fisica").			///< define transformacao
		addConstructor<void(*)()>().
		addFunction("aplicarForca", &bubble::fisica::aplicarForca).
		addFunction("defVelocidade", &bubble::fisica::aplicarVelocidade).
		addFunction("obtVelocidade", &bubble::fisica::obterVelocidade).
		addFunction("defPosicao", &bubble::fisica::definirPosicao).
		addFunction("defRotacao", &bubble::fisica::definirRotacao).
		addFunction("corpoRigido", &bubble::fisica::obterCorpoRigido).
		endClass().
		beginNamespace("fisica").
		addFunction("raioIntersecta", &bubble::raioIntersecta).
		endNamespace();
}