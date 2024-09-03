#include "inspetor.hpp"
#include "src/ui/widgets/caixa_de_texto.hpp"
#include <cstring> // Para std::strncpy
#include <src/tempo/delta_time.hpp>

BubbleUI::Paineis::Inspetor::Inspetor(std::shared_ptr<Contexto> ctx, std::shared_ptr<Bubble::Cena::SceneManager> scenemanager, const Vector4& rect)
    : scenemanager(scenemanager), nome_atual(new std::string(""))
{
    Nome = "Inspetor";
    configurar(ctx, rect);

    // Verifica se o contexto e o scenemanager são válidos antes de usar
    if (scenemanager && ctx)    adiWidget(std::make_shared<Widgets::CaixaTexto>(nome_atual, ""));
    else                        Debug::emitir(Debug::Erro, "Scenemanager ou contexto inválido");// Log de erro
}

BubbleUI::Paineis::Inspetor::~Inspetor() = default;

void BubbleUI::Paineis::Inspetor::recarregar()
{
    lista_widgets.clear();
    // Verifica entidade selecionada e muda nome da entidade atual
    if (entidade_selecionada) nome_atual = entidade_selecionada->nomeptr();
    // Verifica se o contexto e o scenemanager são válidos antes de usar
    adiWidget(std::make_shared<Widgets::CaixaTexto>(nome_atual, "Nome da entidade"));
}

void BubbleUI::Paineis::Inspetor::preAtualizacao()
{
    // Recarrega no momento certo
    if (entidade_selecionada != scenemanager->cenaAtual()->entidade_selecionada)
    {
        entidade_selecionada = scenemanager->cenaAtual()->entidade_selecionada;
        recarregar();
    }
}
