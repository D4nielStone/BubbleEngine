/** @copyright Copyright (c) 2024 Daniel Oliveira */

/** Projetos
*
*	Uma janela para a visualização, edição( deletar, mudar nome etc.) e seleção
*	de projetos da engine.
*/

/* Inclusões */
#include "bubbleui/src/ui/contexto/contexto.hpp"
#include "bubbleui/src/ui/painel/visualizador_de_projetos.hpp"
#include "src/ui/ancoragem/ancora.hpp"
#include <windows.h>

/**
* Funcao de inicializacao
*/

#ifdef NDEBUG
#define INIT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
#else
#define INIT main()
#endif
int INIT
{
	auto contexto = BubbleUI::janela("Bubble Engine - Project Manager - (C) 2024 Daniel Oliveira");
    if (!contexto)return -1;

	contexto->ancora_root->definirPainel(new BubbleUI::Paineis::VisualizadorDeProjetos());

	while (!BubbleUI::fim())
	{
		BubbleUI::atualizarContexto();
		BubbleUI::renderizarContexto();
	}
	return 0;
}
