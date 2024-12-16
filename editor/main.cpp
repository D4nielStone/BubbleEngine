/** @copyright Copyright (c) 2024 Daniel Oliveira */

/// Definindo título da janela
constexpr const char* title = "Bubble Engine - Editor - (C) 2024 Daniel Oliveira";
#include "os/janela.hpp"
#include "src/entidades/entidade.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/componentes/camera.hpp"
#include "src/componentes/transformacao.hpp"
#include "src/nucleo/cena.hpp"
#include <windows.h>
#include <random>

#ifdef _DEBUG
#define INIT main()
#else
#define INIT APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#endif

int INIT
{
	bubble::janela janela(title);
	bubble::cena cena;

	bubble::entidade player = cena.obterRegistro()->criar();

	cena.obterRegistro()->adicionar<bubble::camera>(player, bubble::vetor3(0.f, 3.f, 15.f));

	auto mdl_cubo = new bubble::modelo(R"(C:\Users\DN\3D Objects\cubo\cubo.obj)");
	mdl_cubo->obterMalha(0)->material.uvMundo = true;
	auto mdl_esfera = new bubble::modelo(R"(C:\Users\DN\3D Objects\monkey\monkey.obj)");

	bubble::entidade cubo = cena.obterRegistro()->criar();
	cena.obterRegistro()->adicionar<bubble::transformacao>(cubo, bubble::vetor3(0.f,-4.f,0.f), bubble::vetor3(0.f, 0.f, 0.f), bubble::vetor3(5.f, 0.5f, 5.f));
	cena.obterRegistro()->adicionar<bubble::renderizador>(cubo, mdl_cubo);
	
	bubble::entidade esfera = cena.obterRegistro()->criar();
	cena.obterRegistro()->adicionar<bubble::transformacao>(esfera);
	cena.obterRegistro()->adicionar<bubble::renderizador>(esfera, mdl_esfera);

	cena.definirCamera(player);
	cena.obterCamera()->viewport_ptr = &janela.tamanho;
	cena.iniciar();

	while (!glfwWindowShouldClose(janela.window))
	{
		janela.poll();
		janela.viewport();

		cena.atualizar(0.016666);

		janela.swap();
	}

	return 0;
}