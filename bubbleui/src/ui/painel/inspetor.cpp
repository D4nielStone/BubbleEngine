#include "inspetor.hpp"
#include "src/ui/widgets/caixa_de_texto.hpp"
#include "src/ui/widgets/arvore.hpp"

BubbleUI::Paineis::Inspetor::Inspetor(std::shared_ptr<Contexto> ctx, std::shared_ptr<Bubble::Cena::SceneManager> scenemanager, const Vector4& rect)
    : scenemanager(scenemanager), nome_atual(new std::string(""))
{
    Nome = "Inspetor";
    configurar(ctx, rect);

    // Verifica se o contexto e o scenemanager são válidos antes de usar
    if (scenemanager && ctx)    adicionarWidget(std::make_shared<Widgets::CaixaTexto>(nome_atual, ""));
    else                        Debug::emitir(Debug::Erro, "Scenemanager ou contexto inválido");// Log de erro
}

BubbleUI::Paineis::Inspetor::~Inspetor() = default;

void BubbleUI::Paineis::Inspetor::recarregar()
{
    lista_widgets.clear();
    // Verifica entidade selecionada e muda nome da entidade atual
    if (entidade_selecionada) nome_atual = entidade_selecionada->nomeptr();
    // Verifica se o contexto e o scenemanager são válidos antes de usar
    adicionarWidget(std::make_shared<Widgets::CaixaTexto>(nome_atual, "Nome da entidade"));
    for (const auto& componente : entidade_selecionada->listaDeComponentes())
    {
        adicionarWidget(std::make_shared<Widgets::Arvore>(componente->nome(), nullptr));
    }
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
