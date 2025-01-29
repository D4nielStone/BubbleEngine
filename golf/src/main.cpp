/** @copyright Copyright (c) 2024 Daniel Oliveira */
#include <glad/glad.h>
#include <iostream>
#include "nucleo/fase.hpp"
#include "os/janela.hpp"
#include "os/sistema.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/renderizador.hpp"
#include "componentes/codigo.hpp"
#include "componentes/camera.hpp"

using namespace bubble;

/// Definindo título da janela
int main()
{
	instanciaJanela = new bubble::janela("Golf - (c) Daniel Oliveira");
	fase_atual = new fase((obterExecDir() + "/Golf/level0.fase").c_str());
	fase_atual->iniciar();

	while(!glfwWindowShouldClose(instanciaJanela->window))
	{
		instanciaJanela->poll();

		fase_atual->atualizar(0.01666);

		instanciaJanela->swap();
	}
	return 0;
}