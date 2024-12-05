/** @copyright Copyright (c) 2024 Daniel Oliveira */

// Incluindo bibliotecas necessárias para a interface e motor
#include "src/ui/contexto/contexto.hpp"
#include "src/ui/painel/editor.hpp"
#include "src/ui/painel/depurador.hpp"
#include "src/ui/painel/entidades.hpp"
#include "src/ui/painel/inspetor.hpp"
#include "src/ui/ancoragem/ancora.hpp"
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
    BubbleUI::Paineis::Editor* editor = new BubbleUI::Paineis::Editor( engine.obterGerenciadorDeCenas());
    BubbleUI::Paineis::Entidades* ent = new BubbleUI::Paineis::Entidades( engine.obterGerenciadorDeCenas(), { 3, 3, 200, 400 });
    BubbleUI::Paineis::Inspetor* insp = new BubbleUI::Paineis::Inspetor( engine.obterGerenciadorDeCenas(), { 597, 3, 200, 400 });

    // criando ancoras
    BubbleUI::Ancora* ancora_ent_insp = new BubbleUI::Ancora(BubbleUI::Vertical); ///< alinha  entidades e inspetor

    ancora_ent_insp->a = new BubbleUI::Ancora(BubbleUI::Nenhum, insp);
    ancora_ent_insp->b = new BubbleUI::Ancora(BubbleUI::Nenhum, ent);

    contexto->ancora_root->a = new BubbleUI::Ancora(BubbleUI::Nenhum, editor);
    contexto->ancora_root->b = ancora_ent_insp;

    // Adicionando os painéis à janela do contexto
    engine.definirInputs(contexto->inputs);

}
