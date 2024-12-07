/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "entidades.hpp"
#include "src/ui/widgets/caixa_de_texto.hpp"

BubbleUI::Paineis::Entidades::Entidades(std::shared_ptr < Bubble::Cena::SceneManager> scenemanager, const Vector4<int> &rect) : scenemanager(scenemanager)
{
    Nome = "Entidades";
    retangulo = rect;
}
void BubbleUI::Paineis::Entidades::recarregar()
{
    if (!scenemanager || !scenemanager->cenaAtual())return;

    lista_widgets.clear();
    adicionarWidget(std::make_shared<Widgets::CaixaTexto>("Procurar"));

    quantidade_entidades = scenemanager->cenaAtual()->Entidades.size();
    quantidade_cenas = scenemanager->obterCenas().size();

    for (auto& cena : scenemanager->obterCenas())
    {
        auto arvorecena = (std::make_shared<Widgets::Arvore>(cena->nomeptr(), nullptr ,"assets/texturas/icons/scene.png"));
        adicionarWidget(arvorecena);
        for (auto& entidade : cena->Entidades)
        {
            auto arvoreentidade = std::make_shared<Widgets::Arvore>(entidade->nomeptr(), &entidade->selecionada, "assets/texturas/icons/cube.png");
            arvorecena->adiFilho(arvoreentidade);
            for (auto& filho : entidade->obterFilhos())
            {
                recursivo(filho, arvoreentidade);
            }
        }
    }
}

void BubbleUI::Paineis::Entidades::definirContexto(std::shared_ptr<Contexto>ctx)
{
    Painel::definirContexto(ctx);
    recarregar();
}

void BubbleUI::Paineis::Entidades::preAtualizacao()
{
    if (!scenemanager) return;
    if (contexto->inputs->isKeyPressed(Key::Del)) scenemanager->cenaAtual()->removerEntidade(scenemanager->cenaAtual()->entidade_selecionada);
    // Recarrega no momento certo
    if (scenemanager->cenaAtual() && (quantidade_entidades != scenemanager->cenaAtual()->Entidades.size() || quantidade_cenas != scenemanager->obterCenas().size()))
    {
        recarregar();
    }
}

void BubbleUI::Paineis::Entidades::recursivo(std::shared_ptr<Bubble::Entidades::Entidade> entidade, std::shared_ptr<Widgets::Arvore> arvore)
{
    auto arvore_recursiva = std::make_shared<Widgets::Arvore>(entidade->nomeptr(), &entidade->selecionada,"assets/texturas/icons/cube.png");
    arvore->adiFilho(arvore_recursiva);
    for (auto& filho : entidade->obterFilhos())
    {
        recursivo(filho, arvore_recursiva);
    }
}
