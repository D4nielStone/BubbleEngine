/** @copyright Copyright (c) 2024 Daniel Oliveira */
#include "nucleo/projeto.hpp"
#include "os/sistema.hpp"

/// Definindo título da janela
int main()
{
	bubble::projeto jogo_golf(bubble::obterExecDir() + "/Golf");
	
	jogo_golf.rodar();

	return 0;
}