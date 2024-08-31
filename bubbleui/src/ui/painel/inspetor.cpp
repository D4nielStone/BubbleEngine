#include "inspetor.hpp"
#include "src/ui/widgets/caixa_de_texto.hpp"
#include <cstring> // Para std::strncpy

BubbleUI::Paineis::Inspetor::Inspetor(Contexto* ctx, Bubble::Cena::SceneManager* scenemanager, Vector4 rect)
    : scenemanager(scenemanager), nome_atual(new std::string(""))
{
    Nome = "Inspetor";
    configurar(ctx, rect);

    // Verifica se o contexto e o scenemanager s�o v�lidos antes de usar
    if (scenemanager && ctx)    adiWidget(std::make_shared<Widgets::CaixaTexto>(nome_atual, ""));
    else                        Debug::emitir(Debug::Erro, "Scenemanager ou contexto inv�lido");// Log de erro
}

BubbleUI::Paineis::Inspetor::~Inspetor()
{
    delete[] nome_atual;
}

void BubbleUI::Paineis::Inspetor::recarregar()
{
    lista_widgets.clear();
    // Verifica se h� entidades
    if(!scenemanager->cenaAtual()->Entidades.empty()) nome_atual = scenemanager->cenaAtual()->Entidades.back()->nomeptr();
    // Verifica se o contexto e o scenemanager s�o v�lidos antes de usar
    if (scenemanager && contexto)    adiWidget(std::make_shared<Widgets::CaixaTexto>(nome_atual, ""));
    else                        Debug::emitir(Debug::Erro, "Scenemanager ou contexto inv�lido");// Log de erro
}

void BubbleUI::Paineis::Inspetor::preAtualizacao()
{
    // L�gica de recarregamento
    if (selecionado && !gatilho_recarregar)
    {
        recarregar(); gatilho_recarregar = true;
    }
    if (!selecionado) gatilho_recarregar = false;
}
