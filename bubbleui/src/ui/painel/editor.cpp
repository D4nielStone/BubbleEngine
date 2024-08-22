#include "editor.hpp"

BubbleUI::Paineis::Editor::Editor(Contexto* ctx, Bubble::Cena::SceneManager* scenemanager, Vector4 rect) : scenemanager(scenemanager), buffer(new Widgets::Imagem(0))
{
	Nome = "Editor";
	renderizar_corpo = false;
	configurar(ctx, rect);
	adiWidget(buffer);
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
