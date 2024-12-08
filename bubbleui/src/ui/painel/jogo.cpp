
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "jogo.hpp"
#include "src/ui/items/item_botao.hpp"
#include "src/ui/items/item_arvore.hpp"
#include <windows.h>
#include <filesystem>

using namespace BubbleUI::Widgets;
using namespace BubbleUI::Paineis;
Jogo::Jogo(std::shared_ptr<Bubble::Cena::SceneManager> scenemanager, const Vector4<int>& rect) : buffer(std::make_shared<Imagem>(0))
, scenemanager(scenemanager)
{
	Nome = "Preview camera";
	renderizarCorpo = false;
    buffer->flip = true;
}
void Jogo::definirContexto(std::shared_ptr<Contexto>ctx)
{
    Painel::definirContexto(ctx);
        adicionarWidget(buffer);
}

void BubbleUI::Paineis::Jogo::preAtualizacao()
{
    Vector4<int> rect_size = buffer->obtRect();
    scenemanager->defJogoViewport(rect_size);
    widgetPadding = { 0, -1 };
    buffer->defID(0);
    (scenemanager->cenaAtual() && scenemanager->cenaAtual()->camera_principal) && buffer->defID(scenemanager->cenaAtual()->camera_principal->textureColorbuffer);
        

    if (selecionado) {
        contexto->inputs->setInputMode(Game);
    }
    else if(contexto->inputs->getInputMode() != Editor) contexto->inputs->setInputMode(Default);

    // Detecta toque do ctrl F5 para iniciar janela
    if (contexto->inputs->getInputMode() == Game && contexto->inputs->isKeyPressed(Ctrl) && contexto->inputs->isKeyPressed(F5))
        Debug::emitir("Game", "F5");
}
