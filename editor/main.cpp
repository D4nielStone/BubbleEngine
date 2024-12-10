/** @copyright Copyright (c) 2024 Daniel Oliveira */

/// Definindo título da janela
constexpr const char* title = "Bubble Engine - Editor - (C) 2024 Daniel Oliveira";
#include "os/janela.hpp"
#include "src/entidades/entidade.hpp"
#include "src/componentes/propriedades.hpp"

int main()
{
	Janela janela(title);
	GerenciadorDeEntidades ge;

	Entidade cubo = ge.criarEntidade();
	ge.adicionarComponente<Propriedades>(cubo, "Cubo");
	Entidade player = ge.criarEntidade();
	ge.adicionarComponente<Propriedades>(player, "Player");


	/// Loop principal
	while (!glfwWindowShouldClose(janela.window))
	{
		janela.atualizar();

		janela.desenhar();
	}

	return 0;
}