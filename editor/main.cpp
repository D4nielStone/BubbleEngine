/** @copyright Copyright (c) 2024 Daniel Oliveira */

// Incluindo bibliotecas necess�rias para a interface e motor
#include "src/ui/contexto/contexto.hpp"
#include "src/ui/painel/editor.hpp"
#include "src/ui/painel/depurador.hpp"
#include "src/ui/painel/entidades.hpp"
#include "src/ui/painel/inspetor.hpp"
#include "src/nucleo/engine.hpp"
#include <windows.h>

// Definindo t�tulo da janela
constexpr const char* title = "Bubble Engine - Editor - (C) 2024 Daniel Oliveira";

// Inst�ncia da engine do Bubble
Bubble::Engine engine;

// Defini��o do ponto de entrada dependendo do modo de compila��o (Debug ou Release)
#ifdef NDEBUG
#define INIT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
#else
#define INIT main()
#endif

// Fun��o de inicializa��o da interface gr�fica
void iniciarInterface();

/**
 * Fun��o principal que inicializa o projeto e a interface, e executa o loop do editor.
 */
int INIT
{

// Inicializando a interface do editor
iniciarInterface();

// Loop de execu��o enquanto o editor estiver ativo
while (!BubbleUI::fim())
{
    // Atualizando o contexto gr�fico
    BubbleUI::atualizarContexto();

    // Atualizando a engine (processa a l�gica do jogo/editor)
    engine.atualizar();

    // Renderizando a cena (tanto para o editor quanto para o jogo)
    engine.renderizar();

    // Renderizando o contexto gr�fico da interface
    BubbleUI::renderizarContexto();
}

return 0;
}

/**
 * Fun��o para inicializar os pain�is da interface do editor.
 */
    void iniciarInterface()
{
    // Criando a janela principal do editor
    auto contexto = BubbleUI::janela(title);

    // Carregando o projeto da engine
    if (!engine.carregarProjeto(R"(C:\Users\DN\Documents\Bubble Engine\Projetos\teste)"))
    {
        Debug::emitir(Erro, "carregando projeto.");
        return ;
    }

    // Criando os pain�is da interface do editor
    auto editor = new BubbleUI::Paineis::Editor(engine.obterGerenciadorDeCenas());
    auto ent = new BubbleUI::Paineis::Entidades(engine.obterGerenciadorDeCenas(), { 3, 3, 200, 400 });
    auto insp = new BubbleUI::Paineis::Inspetor(engine.obterGerenciadorDeCenas(), { 597, 3, 200, 400 });

    // Definindo o painel editor para tela cheia
    editor->Fullscreen(true);

    // Adicionando os pain�is � janela do contexto
    BubbleUI::adicionarPainel(contexto.get(), editor);
    BubbleUI::adicionarPainel(contexto.get(), ent);
    BubbleUI::adicionarPainel(contexto.get(), insp);

#ifdef _DEBUG
    // Adicionando o painel de depura��o se estiver no modo de debug
    BubbleUI::adicionarPainel(contexto.get(), new BubbleUI::Paineis::Depurador());
#endif
    
    engine.definirInputs(contexto->inputs);

}
