/** @copyright Copyright (c) 2024 Daniel Oliveira */
#include "nucleo/projeto.hpp"
#include "os/sistema.hpp"
#include "depuracao/debug.hpp"

/// Definindo título da janela
int main(int argv, char* argc[])
{
	std::string projeto_dir = std::filesystem::path(bubble::obterExecDir()).parent_path().parent_path().string() + "/jogos/Golf";

	if(argv > 1)
	{
		projeto_dir = argc[1];
	}
	debug::emitir(Mensagem, "Iniciando projeto em:" + projeto_dir);

	bubble::projeto runtime(projeto_dir);
	
	runtime.rodar();

	return 0;
}