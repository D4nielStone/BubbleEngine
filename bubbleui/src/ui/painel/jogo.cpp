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
	configurar(ctx, rect);
	adicionarWidget(buffer);
}

void BubbleUI::Paineis::Jogo::preAtualizacao()
{
    Vector4 rect_size = buffer->obtRect();
    scenemanager->defJogoViewport(rect_size);

    if (scenemanager->cenaAtual()->camera_principal) {
        buffer->defID(scenemanager->cenaAtual()->camera_principal->textureColorbuffer);
        renderizarCorpo = false;
    }
    else
    {
        renderizarCorpo = true;
    }

    if (selecionado) {
        contexto->inputs->setInputMode(InputMode::Game);
    }
    else             contexto->inputs->setInputMode(InputMode::Default);
}
