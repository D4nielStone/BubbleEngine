/** @copyright Copyright (c) 2024 Daniel Oliveira */

// Incluindo bibliotecas necessárias para a interface e motor
#include "src/ui/contexto/contexto.hpp"
#include "src/ui/painel/editor.hpp"
#include "src/ui/painel/depurador.hpp"
#include "src/ui/painel/entidades.hpp"
#include "src/ui/painel/inspetor.hpp"
#include "src/nucleo/engine.hpp"
#include <windows.h>

// Definindo título da janela
constexpr const char* title = "Bubble Engine - Editor - (C) 2024 Daniel Oliveira";

// Instância da engine do Bubble
Bubble::Engine engine;

// Definição do ponto de entrada dependendo do modo de compilação (Debug ou Release)
#ifdef NDEBUG
#define INIT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
#else
#define INIT main()
#endif

// Função de inicialização da interface gráfica
void iniciarInterface();

/**
 * Função principal que inicializa o projeto e a interface, e executa o loop do editor.
 */
int INIT
{

// Inicializando a interface do editor
iniciarInterface();

// Loop de execução enquanto o editor estiver ativo
while (!BubbleUI::fim())
{
    // Atualizando o contexto gráfico
    BubbleUI::atualizarContexto();

    // Atualizando a engine (processa a lógica do jogo/editor)
    engine.atualizar();

    // Renderizando a cena (tanto para o editor quanto para o jogo)
    engine.renderizar();

    // Renderizando o contexto gráfico da interface
    BubbleUI::renderizarContexto();
}

return 0;
}

/**
 * Função para inicializar os painéis da interface do editor.
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

    // Criando os painéis da interface do editor
    auto editor = new BubbleUI::Paineis::Editor(engine.obterGerenciadorDeCenas());
    auto ent = new BubbleUI::Paineis::Entidades(engine.obterGerenciadorDeCenas(), { 3, 3, 200, 400 });
    auto insp = new BubbleUI::Paineis::Inspetor(engine.obterGerenciadorDeCenas(), { 597, 3, 200, 400 });

    // Definindo o painel editor para tela cheia
    editor->Fullscreen(true);

    // Adicionando os painéis à janela do contexto
    BubbleUI::adicionarPainel(contexto.get(), editor);
    BubbleUI::adicionarPainel(contexto.get(), ent);
    BubbleUI::adicionarPainel(contexto.get(), insp);

#ifdef _DEBUG
    // Adicionando o painel de depuração se estiver no modo de debug
    BubbleUI::adicionarPainel(contexto.get(), new BubbleUI::Paineis::Depurador());
#endif
    
    engine.definirInputs(contexto->inputs);

}
