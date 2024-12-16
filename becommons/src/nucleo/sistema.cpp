#include "sistema.hpp"
#include "src/nucleo/cena.hpp"

void bubble::sistema::inicializar(bubble::cena* cena)
{
	this->cena = cena;
	this->reg= cena->obterRegistro();
}
