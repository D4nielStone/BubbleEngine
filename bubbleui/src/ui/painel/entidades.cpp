#include "entidades.hpp"
#include "src/ui/widgets/caixa_de_texto.hpp"

BubbleUI::Paineis::Entidades::Entidades(Contexto* ctx, Bubble::Cena::SceneManager* scenemanager, Vector4 rect) : scenemanager(scenemanager)
{
	Nome = "Entidades";
	configurar(ctx, rect);
	recarregar();
}

void BubbleUI::Paineis::Entidades::recarregar()
{
	lista_widgets.clear();

	for (auto& entidade : scenemanager->cenaAtual()->Entidades)
	{
		auto arvore = std::make_shared<Widgets::Arvore>(entidade->nome());
		for (auto& filho : entidade->obterFilhos())
		{
			recursivo(filho, *arvore.get());
		}
		adiWidget(arvore);
		numero_entidades = scenemanager->cenaAtual()->Entidades.size();
	}
}

void BubbleUI::Paineis::Entidades::preAtualizacao()
{
	if (numero_entidades != scenemanager->cenaAtual()->Entidades.size() && selecionado)
		recarregar();
}

void BubbleUI::Paineis::Entidades::recursivo(std::shared_ptr<Bubble::Entidades::Entidade> entidade, Widgets::Arvore& arvore)
{
	for (auto& filho : entidade->obterFilhos())
	{
		recursivo(filho, arvore);
	}
}