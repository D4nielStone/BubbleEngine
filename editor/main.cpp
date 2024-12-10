/** @copyright Copyright (c) 2024 Daniel Oliveira */

/// Definindo título da janela
constexpr const char* title = "Bubble Engine - Editor - (C) 2024 Daniel Oliveira";
#include "os/janela.hpp"

int main()
{
	Janela janela(title);

	/// Loop principal
	while (!glfwWindowShouldClose(janela.window))
	{
		janela.atualizar();

		janela.desenhar();
	}

	return 0;
}