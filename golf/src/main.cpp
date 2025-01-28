/** @copyright Copyright (c) 2024 Daniel Oliveira */
#include <glad/glad.h>
#include <iostream>
#include "src/nucleo/fase.hpp"
#include "os/janela.hpp"
#include "src/componentes/transformacao.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/componentes/camera.hpp"

using namespace bubble;

/// Definindo título da janela
int main()
{
	instanciaJanela = new bubble::janela("Golf - (c) Daniel Oliveira");
	bubble::fase teste;

	auto ent = teste.obterRegistro()->criar();
	auto cam = teste.obterRegistro()->criar();
	
	teste.obterRegistro()->adicionar<bubble::transformacao>(ent);
	//teste.obterRegistro()->adicionar<bubble::renderizador>(ent, "~/Modelos/cubo.obj");
	teste.obterRegistro()->adicionar<bubble::camera>(cam);
	teste.definirCamera(cam);
	teste.obterCamera()->viewport_ptr = nullptr;//&instanciaJanela->tamanho;
	teste.iniciar();

	while(!glfwWindowShouldClose(instanciaJanela->window))
	{
		instanciaJanela->poll();

		teste.atualizar(0.01666);

		instanciaJanela->swap();
	}
	system("pause");
	return 0;
}