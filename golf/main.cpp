/** @copyright Copyright (c) 2024 Daniel Oliveira */

/// Definindo título da janela
constexpr const char* title = "Jogo de Golf - (C) 2024 Daniel Oliveira";
#include <glad/glad.h>
#include "os/janela.hpp"
#include "src/entidades/entidade.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/componentes/camera.hpp"
#include "src/componentes/transformacao.hpp"
#include "ui/formas/retangulo.hpp"
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

	cena.obterRegistro()->adicionar<bubble::camera>(player, bubble::camera::ORTHO, bubble::vetor3(5.f, 5.f, 5.f));

	auto mdl_cubo = new bubble::modelo(R"(C:\Users\DN\3D Objects\cubo\cubo.obj)");
	mdl_cubo->obterMalha(0)->material.difusa = {0.69f, 0.85f, 0.55f, 1.f};
	auto mdl_esfera = new bubble::modelo(R"(C:\Users\DN\3D Objects\golf\golf.obj)");

	bubble::entidade cubo = cena.obterRegistro()->criar();
	cena.obterRegistro()->adicionar<bubble::transformacao>(cubo, bubble::vetor3(0.f,-1.5f,0.f), bubble::vetor3(0.f, 0.f, 0.f), bubble::vetor3(5.f, 0.5f, 5.f));
	cena.obterRegistro()->adicionar<bubble::renderizador>(cubo, mdl_cubo);
	
	bubble::entidade esfera = cena.obterRegistro()->criar();
	cena.obterRegistro()->adicionar<bubble::transformacao>(esfera, bubble::vetor3(-4.5f, 0.f, -4.5f));
	cena.obterRegistro()->adicionar<bubble::renderizador>(esfera, mdl_esfera);

	cena.definirCamera(player);
	cena.obterCamera()->viewport_ptr = &janela.tamanho;
	cena.iniciar();

	double elapT{};

	while (!glfwWindowShouldClose(janela.window))
	{
		janela.poll();
		janela.viewport();

		double deltatime = glfwGetTime() - elapT;
		elapT = glfwGetTime();
		cena.atualizar(deltatime);

		janela.swap();
	}

	return 0;
}