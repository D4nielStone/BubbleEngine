/** @copyright Copyright (c) 2024 Daniel Oliveira */

/** Editor
*
*	
*/

/* Inclusões */
#include "src/ui/contexto/contexto.hpp"
#include "src/ui/painel/editor.hpp"
#include "src/ui/painel/depurador.hpp"
#include "src/ui/painel/entidades.hpp"
#include "src/ui/painel/inspetor.hpp"
#include <Windows.h>

/* Declarações globais */
constexpr const char* title = "Bubble Engine - Editor - (C) 2024 Daniel Oliveira";

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
	auto gerenciador_cenas = std::make_shared<Bubble::Cena::SceneManager>();
	gerenciador_cenas->novaCena("Cena de teste", false);
	gerenciador_cenas->defIputs(contexto->inputs);

	Bubble::Cena::definirSceneManager(gerenciador_cenas);
	Bubble::Cena::criarEntidade(R"(C:\Users\DN\3D Objects\86-bookshelf\Bookshelf\Bookshelf.fbx)");

	auto editor = new BubbleUI::Paineis::Editor(gerenciador_cenas);
	auto ent = new BubbleUI::Paineis::Entidades(gerenciador_cenas, { 3, 3, 200, 400});
	auto insp = new BubbleUI::Paineis::Inspetor(gerenciador_cenas, { 597, 3, 200, 400});
	editor->Fullscreen(true);

	BubbleUI::adicionarPainel(contexto.get(), editor);
	BubbleUI::adicionarPainel(contexto.get(), ent);
	BubbleUI::adicionarPainel(contexto.get(), insp);
#ifdef _DEBUG
	BubbleUI::adicionarPainel(contexto.get(), new BubbleUI::Paineis::Depurador());
#endif
	while (!BubbleUI::fim())
	{
		BubbleUI::atualizarContexto();
		gerenciador_cenas->atualizarCenaAtual();
		gerenciador_cenas->renderizarCenaAtual();
		BubbleUI::renderizarContexto();
	}

	return 0;
}
