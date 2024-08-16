#include "painel.hpp"

BubbleUI::Painel::Painel(Contexto* ctx)
{
	configurar(ctx);
}

BubbleUI::Painel::Painel(Vector4 rect, Contexto* ctx)
{
	configurar(ctx, rect);
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
	preAtualizacao();

	borda_c->atualizar(deltaTime);
	borda_b->atualizar(deltaTime);
	borda_e->atualizar(deltaTime);
	borda_d->atualizar(deltaTime);

	corrigirLimite();


	if (renderizar_corpo)
	{
		corpo_rect->defPos({ retangulo.x, retangulo.y });
		corpo_rect->defTam({ static_cast<float>(retangulo.w), static_cast<float>(retangulo.h) });
		corpo_rect->atualizar(deltaTime);
	}

	m_aba->atualizar(deltaTime);
	for (Widget* widget : lista_widgets)
	{
		widget->atualizar(deltaTime);
	}

	arrastando = false;
}

void BubbleUI::Painel::renderizar()
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(retangulo.x, (contexto->tamanho.height - retangulo.y - retangulo.h), retangulo.w, retangulo.h);

	if (renderizar_corpo)
		corpo_rect->renderizar(GL_TRIANGLES);
	preRenderizacao();
	for (auto& widget : lista_widgets)
	{
		widget->renderizar();
	}
	m_aba->renderizar();

	glDisable(GL_SCISSOR_TEST);

	borda_c->renderizar();
	borda_b->renderizar();
	borda_e->renderizar();
	borda_d->renderizar();
}

void BubbleUI::Painel::configurar(Contexto* ctx, Vector4 rect)
{
	// Customizacao do painel
	contexto = ctx;
	widget_padding = {5, 5};
	limite_min_tam = { 100, 50 };
	retangulo = rect;
	corpo_rect = new Formas::Rect(retangulo, ctx);
	corpo_rect->defCor({ 0.23f, 0.23f, 0.23f });
	borda_c = new Borda(CIMA, this);
	borda_b = new Borda(BAIXO, this);
	borda_e = new Borda(ESQUERDA, this);
	borda_d = new Borda(DIREITA, this);
	m_aba = new Aba(this);
}

void BubbleUI::Painel::preAtualizacao()
{
}

void BubbleUI::Painel::preRenderizacao()
{
}

// Defini limite para redimensionamento
void BubbleUI::Painel::corrigirLimite()
{
	if (retangulo.w < limite_min_tam.x && redimen_atual == DIREITA)
	{
		retangulo = { retangulo.x, retangulo.y, limite_min_tam.x, retangulo.h };
	}
	if (retangulo.h < limite_min_tam.y && redimen_atual == BAIXO)
	{
		retangulo = { retangulo.x, retangulo.y, retangulo.w, limite_min_tam.y };
	}
	if (retangulo.w < limite_min_tam.x && redimen_atual == ESQUERDA)
	{
		retangulo = { retangulo.x + retangulo.w - limite_min_tam.x, retangulo.y , retangulo.w, retangulo.h};
		retangulo = { retangulo.x, retangulo.y , limite_min_tam.x, retangulo.h };
	}
	if (retangulo.h < limite_min_tam.y && redimen_atual == CIMA)
	{
		retangulo = {retangulo.x, retangulo.y + retangulo.h - limite_min_tam.y , 0, 0};
		retangulo = { retangulo.x, retangulo.y, retangulo.w, limite_min_tam.y };
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

std::string BubbleUI::Painel::nome() const
{
	return Nome;
}
