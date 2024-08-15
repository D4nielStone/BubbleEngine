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
		scenemanager->cenaAtual()->camera_editor.desenharFrame(rect_size);

		scenemanager->renderizarCenaAtual(rect_size);

		// Desligar framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		buffer->defID(scenemanager->cenaAtual()->camera_editor.textureColorbuffer);
	}
}
