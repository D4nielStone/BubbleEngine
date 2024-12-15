/** @copyright Copyright (c) 2024 Daniel Oliveira */

/// Definindo título da janela
constexpr const char* title = "Bubble Engine - Editor - (C) 2024 Daniel Oliveira";
#include "os/janela.hpp"
#include "src/entidades/entidade.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/componentes/camera.hpp"
#include "src/componentes/transformacao.hpp"
#include <random>

int main()
{
	Janela janela(title);
	GerenciadorDeEntidades ge;

	Entidade player = ge.criarEntidade();

	ge.adicionarComponente<Camera>(player, Vetor3(0.f, 0.f, -10.F),Vetor3(0.f, 0.f, 0.F), 75.f, 1.4F, 0.1F, 400.F);
	
	const int numEnt = 300;

	auto modelo = new Modelo(R"(C:\Users\DN\3D Objects\cubo\cubo.obj)");


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

		Entidade cubo = ge.criarEntidade();
		ge.adicionarComponente<Transformacao>(cubo, Vetor3<float>( x, y, z ), Vetor3<float>(rot_x, rot_y, rot_z),Vetor3( 0.5f, 0.5f, 0.5f ));
		ge.adicionarComponente<Renderizador>(cubo, modelo);
		janela.adicionarTarefa(cubo.id, [&](uint32_t id)
			{
				auto t = ge.obterComponete<Transformacao>(id);
				t->rotacao.y++;
			}
		);
	}

	janela.adicionarSistema<SistemaDeRenderizacao>(&ge);

	janela.iniciarLoop();

	return 0;
}