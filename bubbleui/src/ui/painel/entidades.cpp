#include "entidades.hpp"
#include "src/ui/widgets/texto.hpp"

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
	if (scenemanager->cenaAtualIdx() != -1)
	{
		for (auto& entidade : scenemanager->cenaAtual()->Entidades)
		{
			adiWidget(new Widgets::Texto(entidade->nome(), 12));
			adiWidget(new Widgets::Texto("\n", 12));
		}
	}
	else
	{
		Debug::emitir(Debug::Erro, "Painel entidades: Sem cena Atual");
	}
}

void BubbleUI::Paineis::Entidades::preAtualizacao()
{
}
