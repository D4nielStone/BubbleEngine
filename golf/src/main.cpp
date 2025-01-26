/** @copyright Copyright (c) 2024 Daniel Oliveira */
#include <iostream>
#include "os/janela.hpp"

using namespace bubble;

/// Definindo t�tulo da janela
int main()
{
	instanciaJanela = new bubble::janela("Golf - (c) Daniel Oliveira");

	while(!glfwWindowShouldClose(instanciaJanela->window))
	{
		instanciaJanela->poll();
		instanciaJanela->swap();
	}
	return 0;
}