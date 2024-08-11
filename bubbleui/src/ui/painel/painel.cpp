#include "painel.hpp"

BubbleUI::Painel::Painel(Contexto* ctx) : corpo_rect(new Formas::Rect({100, 100, 300, 150}, ctx)), contexto(ctx)
{
	// Customizacao do painel
	retangulo = { 100, 100, 300, 150 };
	corpo_rect->defCor({0.23f, 0.23f, 0.23f});
	borda_c = new Borda(CIMA, this);
	borda_b = new Borda(BAIXO, this);
	borda_e = new Borda(ESQUERDA, this);
	borda_d = new Borda(DIREITA, this);
}

void BubbleUI::Painel::defTam(Vector2f tam)
{
	retangulo.z = tam.x;
	retangulo.w = tam.y;
}

void BubbleUI::Painel::defPos(Vector2f pos)
{
	retangulo.x = pos.x;
	retangulo.y = pos.y;
}

Vector4f BubbleUI::Painel::obtRect() const
{
	return retangulo;
}

BubbleUI::Contexto* BubbleUI::Painel::obtCtx() const
{
	return contexto;
}

void BubbleUI::Painel::atualizar(float deltaTime)
{
	corpo_rect->defPos({ retangulo.x, retangulo.y });
	corpo_rect->defTam({ retangulo.z, retangulo.w });
	corpo_rect->atualizar(deltaTime);

	// Verifica se nenhuma borda está tocada pelo cursor e muda para cursor normal
	if (!borda_e->cursor()
		&& !borda_d->cursor()
		&& !borda_c->cursor()
		&& !borda_b->cursor())
	{
		contexto->cursor = contexto->cursor_normal;
	}

	borda_c->atualizar(deltaTime);
	borda_b->atualizar(deltaTime);
	borda_e->atualizar(deltaTime);
	borda_d->atualizar(deltaTime);
}

void BubbleUI::Painel::renderizar()
{
	corpo_rect->renderizar(GL_TRIANGLES);
	borda_c->renderizar();
	borda_b->renderizar();
	borda_e->renderizar();
	borda_d->renderizar();
}