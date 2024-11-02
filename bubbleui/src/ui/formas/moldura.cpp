#include "moldura.hpp"

// Define a espessura da borda
int espessuraBorda = 8;

BubbleUI::Formas::Moldura::Moldura(std::shared_ptr<Contexto> contexto, const Vector4& retangulo)
{
	configurar(contexto, retangulo);
}

void BubbleUI::Formas::Moldura::configurar(std::shared_ptr<BubbleUI::Contexto> contexto, const Vector4& retangulo)
{

	this->contexto = contexto;
	this->retangulo = retangulo;
	Rect::Rect(contexto, retangulo);
	*cor_base = { 0.3f, 0.3f, 0.3f, 1.f };
	borda_d = std::make_unique<Rect>(contexto, Vector4{ 0, 0, 0, 0 });
	borda_b = std::make_unique<Rect>(contexto, Vector4{ 0, 0, 0, 0 });
	borda_e = std::make_unique<Rect>(contexto, Vector4{ 0, 0, 0, 0 });
	borda_c = std::make_unique<Rect>(contexto, Vector4{ 0, 0, 0, 0 });
	borda_d->defCor(cor_base);
	borda_b->defCor(cor_base);
	borda_e->defCor(cor_base);
	borda_c->defCor(cor_base);
	ponta_a = std::make_unique<MeioCirculo>(contexto, Vector4{ 0, 0, espessuraBorda, espessuraBorda });
	ponta_b = std::make_unique<MeioCirculo>(contexto, Vector4{ 0, 0, -espessuraBorda, espessuraBorda });
	ponta_c = std::make_unique<MeioCirculo>(contexto, Vector4{ 0, 0, -espessuraBorda, -espessuraBorda });
	ponta_d = std::make_unique<MeioCirculo>(contexto, Vector4{ 0, 0, espessuraBorda, -espessuraBorda });
	ponta_a->defCor(cor_base);
	ponta_b->defCor(cor_base);
	ponta_c->defCor(cor_base);
	ponta_d->defCor(cor_base);
}

void BubbleUI::Formas::Moldura::atualizar()
{
	preAtualizacao();
	
	Rect::atualizar();
	if (ocultar_linhas)
		return;

	// Ponta direita cima
	ponta_a->defPos({ static_cast<int>(retangulo.x) + retangulo.w, static_cast<int>(retangulo.y) + retangulo.h });
	ponta_b->defPos({ static_cast<int>(retangulo.x), static_cast<int>(retangulo.y) + retangulo.h });
	ponta_c->defPos({ static_cast<int>(retangulo.x), static_cast<int>(retangulo.y) });
	ponta_d->defPos({ static_cast<int>(retangulo.x) + retangulo.w, static_cast<int>(retangulo.y)});
	// Borda direita
	borda_d->definirRetangulo({
		retangulo.x + retangulo.w,
		retangulo.y,
		espessuraBorda,
		retangulo.h
		});

	// Borda esquerda
	borda_e->definirRetangulo({
		retangulo.x - espessuraBorda,
		retangulo.y,
		espessuraBorda,
		retangulo.h
		});

	// Borda superior
	borda_c->definirRetangulo({
		retangulo.x,
		retangulo.y - espessuraBorda,
		retangulo.w,
		espessuraBorda
		});

	// Borda inferior
	borda_b->definirRetangulo({
		retangulo.x,
		retangulo.y + retangulo.h,
		retangulo.w,
		espessuraBorda
		});
	borda_b->atualizar();
	borda_c->atualizar();
	borda_d->atualizar();
	borda_e->atualizar();
	ponta_b->atualizar();
	ponta_c->atualizar();
	ponta_d->atualizar();
	ponta_a->atualizar();
	retangulo_completo = { retangulo.x - espessuraBorda, retangulo.y - espessuraBorda, retangulo.w + espessuraBorda * 2, retangulo.h + espessuraBorda * 2};
	posAtualizacao();
}

void BubbleUI::Formas::Moldura::renderizar() const
{
	Rect::renderizar();
	if (ocultar_linhas)return;
	borda_c->renderizar();
	borda_e->renderizar();
	borda_d->renderizar();
	borda_b->renderizar();
	ponta_c->renderizar();
	ponta_a->renderizar();
	ponta_d->renderizar();
	ponta_b->renderizar();
}

void BubbleUI::Formas::Moldura::defTam(const Vector2& tam)
{
	retangulo.w = tam.x - espessuraBorda * 2;
	retangulo.h = tam.y - espessuraBorda * 2;
}
void BubbleUI::Formas::Moldura::defPos(const Vector2& pos)
{
	retangulo.x = pos.x + espessuraBorda;
	retangulo.y = pos.y + espessuraBorda;
}

void BubbleUI::Formas::Moldura::definirRetangulo(const Vector4& rect)
{
	if (!ocultar_linhas) {
		retangulo.w = rect.w - espessuraBorda * 2;
		retangulo.h = rect.h - espessuraBorda * 2;
		retangulo.x = rect.x + espessuraBorda;
		retangulo.y = rect.y + espessuraBorda;
	}
	else retangulo = rect;
}

Vector4 BubbleUI::Formas::Moldura::obtRect() const
{
	if (!ocultar_linhas)
		return retangulo_completo;
	else return retangulo;
}

std::shared_ptr<BubbleUI::Contexto> BubbleUI::Formas::Moldura::obtCtx() const
{
	return contexto;
}