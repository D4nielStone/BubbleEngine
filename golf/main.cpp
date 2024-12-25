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
#include <src/componentes/texto.hpp>
#include <src/componentes/codigo.hpp>

#ifdef _DEBUG
#define INIT main()
#else
#define INIT APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#endif 

int INIT
{
	bubble::janela janela(title, "icon_golf.ico");
	bubble::cena cena;
	cena_atual = &cena;

	bubble::entidade player = cena.obterRegistro()->criar();
	bubble::entidade cubo = cena.obterRegistro()->criar();
	bubble::entidade bandeira = cena.obterRegistro()->criar();
	bubble::entidade level_txt = cena.obterRegistro()->criar();
	bubble::entidade bola = cena.obterRegistro()->criar();

	auto mdl_cubo = new bubble::modelo(R"(C:\Users\DN\3D Objects\cubo\cubo.obj)");
	auto mdl_bandeira = new bubble::modelo(R"(C:\Users\DN\3D Objects\golf\golf.obj)");
	auto mdl_bola = new bubble::modelo(R"(C:\Users\DN\3D Objects\golf\bola\scene.gltf)");

	cena.obterRegistro()->adicionar<bubble::camera>(player, bubble::vetor3(5.f, 5.f, 5.f), bubble::camera::f_Alvo);

	cena.obterRegistro()->adicionar<bubble::transformacao>(cubo);
	cena.obterRegistro()->adicionar<bubble::renderizador>(cubo, mdl_cubo);

	cena.obterRegistro()->adicionar<bubble::transformacao>(bola, bubble::vetor3{0.f, 0.f, 8.f}, bubble::vetor3{ 0.f, 0.f, 0.f }, bubble::vetor3{ 0.1f, 0.1f, 0.1f });
	cena.obterRegistro()->adicionar<bubble::renderizador>(bola, mdl_bola);
	cena.obterRegistro()->adicionar<bubble::codigo>(bola, R"(C:\Users\DN\Desktop\deslizamento_camera.lua)");

	cena.obterRegistro()->adicionar<bubble::transformacao>(bandeira);
	cena.obterRegistro()->adicionar<bubble::renderizador>(bandeira, mdl_bandeira);

	cena.obterRegistro()->adicionar<bubble::texto>(level_txt, "Level 0");

	cena.definirCamera(player);
	cena.obterCamera()->viewport_ptr = &janela.tamanho;
	cena.iniciar();

	double elapT{};

	while (!glfwWindowShouldClose(janela.window))
	{
		janela.poll();

		double deltatime = glfwGetTime() - elapT;
		elapT = glfwGetTime();

		janela.viewport();

		cena.atualizar(deltatime);

		janela.swap();
	}

	return 0;
}