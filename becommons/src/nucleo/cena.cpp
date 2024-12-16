#include "cena.hpp"

bubble::cena::cena()
{
	srender.inicializar(this);
}

void bubble::cena::pausar()
{
	Debug::emitir("cena", "Pausando");
	rodando = false;
}

void bubble::cena::parar()
{
	Debug::emitir("cena", "Parando");
	// TODO: snapshot para retornar o rodando do registro
	rodando = false;
}

void bubble::cena::iniciar()
{
	Debug::emitir("cena", "Iniciando");
	if (rodando != false)
		return;

	// capturar snapshot do registro
	rodando = true;
}

double elapsedTime;

void bubble::cena::atualizar(double deltaTime)
{
	if (rodando == false)
	{
		// se parado apenas renderiza
		srender.atualizar(deltaTime);
	}
	else
	{
		// efetua operacoes matematicas de transformacao
		elapsedTime += deltaTime;
		if(elapsedTime >= 1 && rodando) Debug::emitir("cena", "Tempo de execucao: " + std::to_string(elapsedTime) + "s");
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
