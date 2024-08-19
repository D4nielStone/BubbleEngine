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

void BubbleUI::Painel::defPos(Vector2f pos)
{
	retangulo.x = pos.x;
	retangulo.y = pos.y;
}

void BubbleUI::Painel::adiTam(Vector2 tam)
{
	retangulo.w += tam.x;
	retangulo.h += tam.y;
}

void BubbleUI::Painel::adiPos(Vector2f pos)
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

	borda_c->atualizar();
	borda_b->atualizar();
	borda_e->atualizar();
	borda_d->atualizar();

	corrigirLimite();
	

	moldura->defPos({ retangulo.x, retangulo.y });
	moldura->defTam({ (float)retangulo.w,(float)retangulo.h });
	moldura->atualizar(deltaTime);

	preAtualizacao();
	m_aba->atualizar(deltaTime);
	for (Widget* widget : lista_widgets)
	{
		widget->atualizar(deltaTime);
	}

	arrastando = false;
}

void BubbleUI::Painel::renderizar()
{
	glScissor(retangulo.x -1, (contexto->tamanho.height - (retangulo.y - 1) - retangulo.h), retangulo.w + 1, retangulo.h + 1);

	moldura->renderizar(GL_TRIANGLES);
	preRenderizacao();
	for (auto& widget : lista_widgets)
	{
		widget->renderizar();
	}
	m_aba->renderizar();

}

void BubbleUI::Painel::configurar(Contexto* ctx, Vector4 rect)
{
	// Customizacao do painel
	contexto = ctx;
	widget_padding = {5, 5};
	limite_min_tam = { 100, 15 };
	retangulo = rect;
	borda_c = new Separador(CIMA, this);
	borda_b = new Separador(BAIXO, this);
	borda_e = new Separador(ESQUERDA, this);
	borda_d = new Separador(DIREITA, this);
	moldura = new Formas::Moldura(contexto);
	moldura->defCor({ 0.2f, 0.2f, 0.2f });
	m_aba = new Aba(this);
}

void BubbleUI::Painel::preAtualizacao()
{
	widget_pos.x += widget_padding.x;
	widget_pos.y += widget_padding.y;
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
		retangulo = {retangulo.x, retangulo.y + retangulo.h - limite_min_tam.y , retangulo.w, retangulo.h};
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
