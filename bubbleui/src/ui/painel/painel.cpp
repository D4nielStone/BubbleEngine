#include "painel.hpp"
#include <src/tempo/delta_time.cpp>

BubbleUI::Painel::Painel(std::shared_ptr<Contexto> ctx, const Vector4& rect)
{
	configurar(ctx, rect);
}

void BubbleUI::Painel::defTam(const Vector2 &tam)
{
	retangulo.w = tam.x;
	retangulo.h = tam.y;
}

void BubbleUI::Painel::defPos(const Vector2 &pos)
{
	retangulo.x = pos.x;
	retangulo.y = pos.y;
}

void BubbleUI::Painel::adiTam(const Vector2 &tam)
{
	retangulo.w += tam.x;
	retangulo.h += tam.y;
}

void BubbleUI::Painel::adiPos(const Vector2 &pos)
{
	retangulo.x += pos.x;
	retangulo.y += pos.y;
}

void BubbleUI::Painel::adiWidget(std::shared_ptr<Widget> widget)
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

std::shared_ptr<BubbleUI::Contexto> BubbleUI::Painel::obtCtx() const
{
	return contexto;
}

void BubbleUI::Painel::atualizar()
{
	if (selecionado)
	{
		borda_c->atualizar(); borda_b->atualizar(); borda_e->atualizar(); borda_d->atualizar();
	}
	corrigirLimite();
	

	moldura.defPos({ static_cast<int>(retangulo.x), static_cast<int>(retangulo.y )});
	moldura.defTam({retangulo.w,retangulo.h });
	moldura.atualizar();

	if (mostrar_popup)
	{
		menu_de_contexto->mostrar();
		mostrar_popup = false;
	}
	else if (esconder_popup)
	{
		menu_de_contexto->esconder();
		esconder_popup = false;
	}

	menu_de_contexto->atualizar();

	if (!selecionado)
	{
		menu_de_contexto->esconder();
		m_aba->obterCorpo()->defCor({0.1f, 0.1f, 0.1f, 1});
	}
	else
	{
		m_aba->obterCorpo()->defCor({0.4f, 0.0f, 0.4f, 1});
	}

	widget_pos = { 0, 0 };
	preAtualizacao();
	m_aba->atualizar();
	for (auto& widget : lista_widgets)
	{
		widget->atualizar();
	}
	posAtualizacao();

	arrastando = false;
}

void BubbleUI::Painel::renderizar() const
{
	glScissor(retangulo.x, (contexto->tamanho.height - (retangulo.y-1) - retangulo.h), retangulo.w, retangulo.h+4);

	moldura.renderizar();
	preRenderizacao();
	m_aba->renderizar();
	for (auto& widget : lista_widgets)
	{
		widget->renderizar();
	}

	menu_de_contexto->renderizar();
}

void BubbleUI::Painel::configurar(std::shared_ptr<Contexto> ctx, const Vector4& rect)
{
	// configura do painel
	contexto = ctx;
	widget_padding = {5, 5};
	limite_min_tam = { 100, 15 };
	retangulo = rect;
	borda_c = std::make_unique<Separador>(CIMA, this);
	borda_b = std::make_unique<Separador>(BAIXO, this);
	borda_e = std::make_unique<Separador>(ESQUERDA, this);
	borda_d = std::make_unique<Separador>(DIREITA, this);
	moldura = Formas::Moldura(contexto);
	menu_de_contexto = std::make_unique<Util::PopUp>(contexto);
	moldura.defCor({ 0.1f, 0.1f, 0.1f });
	m_aba = std::make_unique<Aba>(this);
}

void BubbleUI::Painel::preAtualizacao()
{
}

void BubbleUI::Painel::posAtualizacao()
{
}

void BubbleUI::Painel::preRenderizacao() const
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
bool BubbleUI::Painel::cursorNormal() const
{
	if (!borda_e->cursor()
		&& !borda_b->cursor()
		&& !borda_c->cursor()
		&& !borda_d->cursor())
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