/** @copyright Copyright (c) 2024 Daniel Oliveira */

/// Definindo t�tulo da janela
#include <glad/glad.h>
#include "os/janela.hpp"
#include "../entidades/entidade.hpp"
#include "../componentes/renderizador.hpp"
#include "../componentes/camera.hpp"
#include "../componentes/transformacao.hpp"
#include "ui/formas/retangulo.hpp"
#include "../nucleo/fase.hpp"
#include <random>
#include "../componentes/texto.hpp"
#include "../componentes/codigo.hpp"
#include "../componentes/fisica.hpp"

const std::string title = "Golf - (C) 2025 Daniel Oliveira | ";
using namespace bubble;

int main()
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