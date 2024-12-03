/** @copyright Copyright (c) 2024 Daniel Oliveira */

/** Projetos
*
*	Uma janela para a visualiza��o, edi��o( deletar, mudar nome etc.) e sele��o
*	de projetos da engine.
*/

/* Inclus�es */
#include "bubbleui/src/ui/contexto/contexto.hpp"
#include "bubbleui/src/ui/painel/visualizador_de_projetos.hpp"

/* Declara��es globais */
GLFWwindow* janela{ nullptr };
std::shared_ptr<BubbleUI::Contexto> contexto_ui{ nullptr };
constexpr const char* title = "Bubble Engine - Project Manager - (C) 2024 Daniel Oliveira";

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
	auto contexto = BubbleUI::janela(title);
    if (!contexto)return -1;

	BubbleUI::adicionarPainel(contexto, new BubbleUI::Paineis::VisualizadorDeProjetos());

	while(!BubbleUI::fim())
		BubbleUI::atualizarContexto();

	return 0;
}
