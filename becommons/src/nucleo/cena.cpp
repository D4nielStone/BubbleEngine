#include "cena.hpp"

bubble::cena::cena()
{
	srender.inicializar(this);
}

void bubble::cena::pausar()
{
	estado = estadoDeJogo::PARADO;
}

void bubble::cena::parar()
{
	// TODO: snapshot para retornar o estado do registro
	estado = estadoDeJogo::PARADO;
}

void bubble::cena::iniciar()
{
	if (estado != estadoDeJogo::PARADO)
		return;

	// capturar snapshot do registro
	estado = estadoDeJogo::RODANDO;
}

void bubble::cena::atualizar(double deltaTime)
{
	if (estado == estadoDeJogo::PARADO)
	{
		// se parado apenas renderiza
		srender.atualizar(deltaTime);
	}
	else
	{
		// efetua operacoes matematicas de transformacao
		srender.atualizar(deltaTime);
	}
}

void bubble::cena::definirCamera(const entidade& ent)
{
	if (reg.tem<camera>(ent.id))
		camera_atual = reg.obter<camera>(ent.id);
}

std::shared_ptr<bubble::camera> bubble::cena::obterCamera() const
{
	return camera_atual;
}

bubble::registro* bubble::cena::obterRegistro()
{
	return &reg;
}
