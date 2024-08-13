#include "painel.hpp"

BubbleUI::Painel::Painel(Contexto* ctx) : 
	corpo_rect(new Formas::Rect({2, 2, 50, 50}, ctx)),
	contexto(ctx), retangulo({ 2, 2, 50, 50 }),
	limite_min_tam({ 50, 50 }),
	selecionado(false)
{
	// Customizacao do painel
	corpo_rect->defCor({0.23f, 0.23f, 0.23f});
	borda_c = new Borda(CIMA, this);
	borda_b = new Borda(BAIXO, this);
	borda_e = new Borda(ESQUERDA, this);
	borda_d = new Borda(DIREITA, this);
}

BubbleUI::Painel::Painel(Vector4 rect, Contexto* ctx) : 
	corpo_rect(new Formas::Rect(rect, ctx)),
	contexto(ctx), retangulo(rect),
	limite_min_tam({ 50, 50 }),
	selecionado(false)
{
	// Customizacao do painel
	corpo_rect->defCor({0.23f, 0.23f, 0.23f});
	borda_c = new Borda(CIMA, this);
	borda_b = new Borda(BAIXO, this);
	borda_e = new Borda(ESQUERDA, this);
	borda_d = new Borda(DIREITA, this);
}

void BubbleUI::Painel::defTam(Vector2 tam)
{
	retangulo.w = tam.x;
	retangulo.h = tam.y;
}

void BubbleUI::Painel::defPos(Vector2 pos)
{
	retangulo.x = pos.x;
	retangulo.y = pos.y;
}

void BubbleUI::Painel::adiTam(Vector2 tam)
{
	retangulo.w += tam.x;
	retangulo.h += tam.y;
}

void BubbleUI::Painel::adiPos(Vector2 pos)
{
	retangulo.x += pos.x;
	retangulo.y += pos.y;
}

void BubbleUI::Painel::adiWidget(Widget* widget)
{
	widget->defPainel(this);
	lista_widgets.push_back(widget);
}

Vector4 BubbleUI::Painel::obtRect() const
{
	return retangulo;
}

Vector2 BubbleUI::Painel::obtTamMin() const
{
	return limite_min_tam;
}

BubbleUI::Contexto* BubbleUI::Painel::obtCtx() const
{
	return contexto;
}

void BubbleUI::Painel::atualizar(float deltaTime)
{
	corrigirLimite();

	corpo_rect->defPos({ retangulo.x, retangulo.y });
	corpo_rect->defTam({ static_cast<float>(retangulo.w), static_cast<float>(retangulo.h) });
	corpo_rect->atualizar(deltaTime);

	arrastando = false;

	for (Widget* widget : lista_widgets)
	{
		widget->atualizar(deltaTime);
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
	for (Widget* widget : lista_widgets)
	{
		widget->renderizar();
	}
}

// Defini limite para redimensionamento
void BubbleUI::Painel::corrigirLimite()
{
	if (retangulo.w < limite_min_tam.x && redimen_atual == DIREITA)
	{
		defTam({ limite_min_tam.x, retangulo.h });
	}
	if (retangulo.h < limite_min_tam.y && redimen_atual == BAIXO)
	{
		defTam({ retangulo.w, limite_min_tam.y });
	}
	if (retangulo.w < limite_min_tam.x && redimen_atual == ESQUERDA)
	{
		defPos({ static_cast<int>(retangulo.x) + retangulo.w - limite_min_tam.x, static_cast<int>(retangulo.y) });
		defTam({ limite_min_tam.x, retangulo.h });
	}
	if (retangulo.h < limite_min_tam.y && redimen_atual == CIMA)
	{
		defPos({ static_cast<int>(retangulo.x), static_cast<int>(retangulo.y) + retangulo.h - limite_min_tam.y });
		defTam({ retangulo.w, limite_min_tam.y });
	}
}

// Verifica se nenhuma borda está tocada pelo cursor e muda para cursor normal
bool BubbleUI::Painel::cursorNormal()
{
	if (!borda_e->cursor()
		&& !borda_d->cursor()
		&& !borda_c->cursor()
		&& !borda_b->cursor())
	{
		return true;
	}
	else
		return false;
}