/** @copyright Copyright (c) 2024 Daniel Oliveira */

/// Definindo título da janela
constexpr const char* title = "Bubble Engine - Editor - (C) 2024 Daniel Oliveira";
#include "os/janela.hpp"
#include "src/entidades/entidade.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/componentes/camera.hpp"
#include "src/componentes/transformacao.hpp"
#include <windows.h>
#include <random>

#ifdef DEBUG
#define INIT main()
#else
#define INIT APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#endif
int INIT
{
	bubble::janela janela(title);
	bubble::registro reg;
	bubble::sistemaRenderizacao sr(&reg);

	bubble::entidade player = reg.criar();

	reg.adicionar<bubble::camera>(player, bubble::vetor3(0.f, 0.f, -10.F),bubble::vetor3(0.f, 0.f, 0.F), 75.f, 1.4F, 0.1F, 400.F);
	
	const int numEnt = 50;

	auto modelo = new bubble::modelo(R"(C:\Users\DN\3D Objects\cubo\cubo.obj)");


	// Cria o gerador de números aleatórios
	std::random_device rd; // Gera uma seed verdadeira, se disponível
	std::mt19937 gen(rd()); // Motor Mersenne Twister com seed
	std::uniform_int_distribution<int> distrib(-30, 30); // Distribuição uniforme no intervalo [-100, 100]
	std::uniform_int_distribution<int> distrib_rot(0, 360); // Distribuição uniforme no intervalo [-100, 100]

	for (size_t i = 0; i < numEnt; i++)
	{
		// Gera um número aleatório pos
		int x = distrib(gen);
		int y = distrib(gen);
		int z = distrib(gen);
		// Gera um número aleatório rot
		int rot_x = distrib_rot(gen);
		int rot_y = distrib_rot(gen);
		int rot_z = distrib_rot(gen);

		bubble::entidade cubo = reg.criar();
		reg.adicionar<bubble::transformacao>(cubo, bubble::vetor3<float>( x, y, z ), bubble::vetor3<float>(rot_x, rot_y, rot_z),bubble::vetor3( 0.5f, 0.5f, 0.5f ));
		reg.adicionar<bubble::renderizador>(cubo, modelo);
	}

	sr.inicializar();

	while (!glfwWindowShouldClose(janela.window))
	{
		janela.poll();
		janela.viewport();

		sr.atualizar(0.016666);

		janela.swap();
	}

	return 0;
}