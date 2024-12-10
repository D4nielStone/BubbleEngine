/** @copyright Copyright (c) 2024 Daniel Oliveira */

/// Definindo título da janela
constexpr const char* title = "Bubble Engine - Editor - (C) 2024 Daniel Oliveira";
#include "os/janela.hpp"
#include "src/entidades/entidade.hpp"
#include "src/componentes/propriedades.hpp"
#include "src/componentes/renderizador.hpp"

int main()
{
	Janela janela(title);
	GerenciadorDeEntidades ge;

	Entidade cubo = ge.criarEntidade();
	ge.adicionarComponente<Propriedades>(cubo, "Cubo");
	Entidade player = ge.criarEntidade();
	ge.adicionarComponente<Propriedades>(player, "Player");

	auto mdl = new Modelo(R"(C:\Users\DN\3D Objects\Spider_Monkey_v1_L2.123c40f743d8-db49-416b-ba8d-6baa7817941e\12958_Spider_Monkey_v1_l2.obj)");

	ge.adicionarComponente<Renderizador>(player, mdl);

	/// Loop principal
	while (!glfwWindowShouldClose(janela.window))
	{
		janela.atualizar();


		ge.atualizarRenderizadores(ge);

		janela.desenhar();
	}

	return 0;
}