/** @copyright Copyright (c) 2024 Daniel Oliveira */

/// Definindo título da janela
#include <Windows.h>
#include <glad/glad.h>
#include "os/janela.hpp"
#include "src/entidades/entidade.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/componentes/camera.hpp"
#include "src/componentes/transformacao.hpp"
#include "ui/formas/retangulo.hpp"
#include "src/nucleo/fase.hpp"
#include <random>
#include <src/componentes/texto.hpp>
#include <src/componentes/codigo.hpp>
#include <src/componentes/fisica.hpp>

const std::string title = "Golf - (C) 2025 Daniel Oliveira | ";
using namespace bubble;

#ifdef _DEBUG
#define INIT int main()
#else
#define INIT int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR cmdline, int cmdshow)
#endif 

INIT
{
	instanciaJanela = new janela(title.c_str(), "icon_golf.ico");

	fase f("Golf/level0.fase");

	instanciaJanela->nome((title + fase_atual->nome()).c_str());

	f.iniciar();

	if(f.obterCamera())
	f.obterCamera()->viewport_ptr = &instanciaJanela->tamanho;

	double elapT{};

	while (!glfwWindowShouldClose(instanciaJanela->window))
	{
		instanciaJanela->poll();

		double deltatime = glfwGetTime() - elapT;
		elapT = glfwGetTime();

		instanciaJanela->viewport();

		f.atualizar(deltatime);

		instanciaJanela->swap();
	}

	return 0;
}