/** @copyright Copyright (c) 2024 Daniel Oliveira */
#include <glad/glad.h>
#include <iostream>
#include "src/nucleo/fase.hpp"
#include "os/janela.hpp"
#include "src/componentes/transformacao.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/componentes/codigo.hpp"
#include "src/componentes/camera.hpp"

using namespace bubble;

/// Definindo título da janela
int main()
{
	instanciaJanela = new bubble::janela("Golf - (c) Daniel Oliveira");
	fase_atual = new fase();

	auto ent = fase_atual->obterRegistro()->criar();
	auto cam = fase_atual->obterRegistro()->criar();
	
	fase_atual->obterRegistro()->adicionar<bubble::transformacao>(ent);
	fase_atual->obterRegistro()->adicionar<bubble::renderizador>(ent, new bubble::modelo("/source/repos/BubbleEngine/golf/cubo.obj"));
	fase_atual->obterRegistro()->adicionar<bubble::transformacao>(cam,  vet3(0, 0, 3));
	fase_atual->obterRegistro()->adicionar<bubble::camera>(cam);
	fase_atual->obterRegistro()->adicionar<bubble::codigo>(cam, "/source/repos/BubbleEngine/golf/cam.lua");
	fase_atual->definirCamera(cam);
	fase_atual->obterCamera()->viewport_ptr = &instanciaJanela->tamanho;
	fase_atual->iniciar();

	while(!glfwWindowShouldClose(instanciaJanela->window))
	{
		instanciaJanela->poll();

		fase_atual->atualizar(0.01666);

		instanciaJanela->swap();
	}
	return 0;
}