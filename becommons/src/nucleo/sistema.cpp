#include "sistema.hpp"
#include "src/nucleo/fase.hpp"

void bubble::sistema::inicializar(bubble::fase* fase)
{
	this->fase = fase;
	this->reg= fase->obterRegistro();
}
