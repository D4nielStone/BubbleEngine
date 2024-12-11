/** @copyright Copyright (c) 2024 Daniel Oliveira */

/// Definindo título da janela
constexpr const char* title = "Bubble Engine - Editor - (C) 2024 Daniel Oliveira";
#include "os/janela.hpp"
#include "src/entidades/entidade.hpp"
#include "src/componentes/propriedades.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/componentes/camera.hpp"

int main()
{
	Janela janela(title);
	GerenciadorDeEntidades ge;

	Entidade cubo = ge.criarEntidade();
	Entidade player = ge.criarEntidade();

	ge.adicionarComponente<Propriedades>(cubo, "Cubo");
	ge.adicionarComponente<Propriedades>(player, "Player");


	ge.adicionarComponente<Camera>(player, Vetor3(0.f, 0.f, -5.F));
	ge.adicionarComponente<Renderizador>(cubo, new Modelo(R"(C:\Users\DN\3D Objects\fence-barricade-03\source\Fence-Barricade_03.blend)"));

	janela.adicionarSistema<SistemaDeRenderizacao>(&ge);

	janela.iniciarLoop();

	return 0;
}