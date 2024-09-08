#include "entidades.hpp"
#include "src/ui/widgets/caixa_de_texto.hpp"

BubbleUI::Paineis::Entidades::Entidades(std::shared_ptr < Contexto>ctx, std::shared_ptr < Bubble::Cena::SceneManager> scenemanager, const Vector4 &rect) : scenemanager(scenemanager)
{
	Nome = "Entidades";
	configurar(ctx, rect);
	recarregar();
}

void BubbleUI::Paineis::Entidades::recarregar()
{
	arvoreCor = { 1.f, 1.f, 1.f, 1 };
	lista_widgets.clear();

	adicionarWidget(std::make_shared<Widgets::CaixaTexto>());
	for (auto& entidade : scenemanager->cenaAtual()->Entidades)
	{
		auto arvore = std::make_shared<Widgets::Arvore>(entidade->nomeptr(), &entidade->selecionada);
		adicionarWidget(arvore);
		for (auto& filho : entidade->obterFilhos())
		{
			recursivo(filho, arvore);
		}
	}
}

void BubbleUI::Paineis::Entidades::preAtualizacao()
{
	// Recarrega no momento certo
	if (quantidade_entidades != scenemanager->cenaAtual()->Entidades.size())
	{
		recarregar();
		quantidade_entidades = scenemanager->cenaAtual()->Entidades.size();
	}
}

void BubbleUI::Paineis::Entidades::recursivo(std::shared_ptr<Bubble::Entidades::Entidade> entidade, std::shared_ptr<Widgets::Arvore> arvore)
{
	auto arvore_recursiva = std::make_shared<Widgets::Arvore>(entidade->nomeptr(), &entidade->selecionada);
	arvore->adiFilho(arvore_recursiva);
	for (auto& filho : entidade->obterFilhos())
	{
		recursivo(filho, arvore_recursiva);
	}
}