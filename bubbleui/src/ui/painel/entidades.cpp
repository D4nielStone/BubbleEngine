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
	for (auto& widget : lista_widgets)
	{
		delete widget;
	}
	lista_widgets.clear();
	adiWidget(new Widgets::CaixaTexto());
}