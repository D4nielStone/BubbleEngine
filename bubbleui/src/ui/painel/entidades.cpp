#include "entidades.hpp"
#include "src/ui/widgets/caixa_de_texto.hpp"
#include "src/ui/widgets/botao.hpp"

BubbleUI::Paineis::Entidades::Entidades(Contexto* ctx, Bubble::Cena::SceneManager* scenemanager, Vector4 rect) : scenemanager(scenemanager)
{
	Nome = "Entidades";
	configurar(ctx, rect);
	recarregar();
}

void BubbleUI::Paineis::Entidades::recarregar()
{
	for (auto& widget : lista_widgets)
	{
		delete widget;
	}
	lista_widgets.clear();
	adiWidget(new Widgets::CaixaTexto("Procurar entidade"));
	for (auto& entidade : scenemanager->cenaAtual()->Entidades)
	{
		adiWidget(new Widgets::Botao(entidade->nome()));
		for (auto& filho : entidade->obterFilhos())
		{
			recursivo(filho);
		}
	}
}

void BubbleUI::Paineis::Entidades::recursivo(std::shared_ptr<Bubble::Entidades::Entidade> entidade)
{
	adiWidget(new Widgets::Botao(entidade->nome()));
	for (auto& filho : entidade->obterFilhos())
	{
		recursivo(filho);
	}
}