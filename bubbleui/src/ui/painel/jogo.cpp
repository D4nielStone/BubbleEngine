#include "jogo.hpp"
#include "src/ui/items/item_botao.hpp"
#include "src/ui/items/item_arvore.hpp"
#include <windows.h>
#include <filesystem>

BubbleUI::Paineis::Jogo::Jogo(std::shared_ptr<Contexto> ctx, std::shared_ptr<Bubble::Cena::SceneManager> scenemanager, const Vector4& rect) : buffer(std::make_shared<BubbleUI::Widgets::Imagem>(0))
, scenemanager(scenemanager)
{
	Nome = "Jogo";
	renderizarCorpo = false;
    buffer->flip = true;
	configurar(ctx, rect);
	adicionarWidget(buffer);
}

void BubbleUI::Paineis::Jogo::preAtualizacao()
{
    Vector4 rect_size = buffer->obtRect();
    scenemanager->defJogoViewport(rect_size);

    (scenemanager->cenaAtual() && scenemanager->cenaAtual()->camera_principal) && buffer->defID(scenemanager->cenaAtual()->camera_principal->textureColorbuffer);

    if (selecionado) {
        contexto->inputs->setInputMode(Game);
    }
    else if(contexto->inputs->getInputMode() != Editor) contexto->inputs->setInputMode(Default);

    // Detecta toque do ctrl F5 para iniciar janela
    if (contexto->inputs->getInputMode() == Game && contexto->inputs->isKeyPressed(Ctrl) && contexto->inputs->isKeyPressed(F5))
        Debug::emitir("Game", "F5");
}
