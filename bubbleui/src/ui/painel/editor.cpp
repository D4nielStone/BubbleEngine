#include "editor.hpp"
#include "src/ui/items/item_menu.hpp"

BubbleUI::Paineis::Editor::Editor(Contexto* ctx, Bubble::Cena::SceneManager* scenemanager, Vector4 rect) : scenemanager(scenemanager), buffer(new Widgets::Imagem(0))
{
	Nome = "Editor";
	renderizar_corpo = false;
	configurar(ctx, rect);
	adiWidget(buffer);
	menu_de_contexto->adiItem(new Items::ItemMenu("Teste item menu"));
	menu_de_contexto->adiItem(new Items::ItemMenu("Teste 2 item menu"));
	menu_de_contexto->adiItem(new Items::ItemMenu("Teste 3 item menu"));
}

void BubbleUI::Paineis::Editor::preAtualizacao()
{
	if (scenemanager->cenaAtualIdx() != -1)
	{
		Vector4 rect_size = buffer->obtRect();
		scenemanager->defViewport(rect_size);
		buffer->defID(scenemanager->cenaAtual()->camera_editor.textureColorbuffer);
	}
}
