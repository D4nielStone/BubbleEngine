#include "painel.hpp"

BubbleUI::Painel::Painel(Contexto* ctx) : corpo_rect(new Formas::Rect({100, 100, 200, 150}, ctx)), contexto(ctx)
{
	// Customizacao do painel
	retangulo = { 100, 100, 200, 150 };
	corpo_rect->defCor({0.23f, 0.23f, 0.23f});
	borda_c = new Borda(CIMA, this);
	borda_b = new Borda(BAIXO, this);
	borda_e = new Borda(ESQUERDA, this);
	borda_d = new Borda(DIREITA, this);
}

void BubbleUI::Painel::defTam(Vector2 tam)
{
	retangulo.z = tam.x;
	retangulo.w = tam.y;
}

void BubbleUI::Painel::defPos(Vector2 pos)
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
