/** @copyright Copyright (c) 2024 Daniel Oliveira */

/** Projetos
*
*	Uma janela para a visualização, edição( deletar, mudar nome etc.) e seleção
*	de projetos da engine.
*/

/* Inclusões */
#include "bubbleui/src/ui/contexto/contexto.hpp"
#include "bubbleui/src/ui/painel/visualizador_de_projetos.hpp"

/* Declarações globais */
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
