#include "moldura.hpp"

BubbleUI::Formas::Moldura::Moldura(Contexto* contexto, Vector4 retangulo)
{
	this->contexto = contexto;
	this->retangulo = retangulo;
	linha_d = std::make_unique<Linha>(Vector4f{0, 0, 0, 0}, contexto);
	linha_b = std::make_unique<Linha>(Vector4f{0, 0, 0, 0}, contexto);
	linha_e = std::make_unique<Linha>(Vector4f{0, 0, 0, 0}, contexto);
	linha_c = std::make_unique<Linha>(Vector4f{0, 0, 0, 0}, contexto);
	linha_d->defCor({ 0.35, 0.35, 0.35 });
	linha_b->defCor({ 0.35, 0.35, 0.35 });
	linha_e->defCor({ 0.55, 0.55, 0.55 });
	linha_c->defCor({ 0.55, 0.55, 0.55 });
}

void BubbleUI::Formas::Moldura::atualizar()
{
	preAtualizacao();
	Rect::atualizar();
	if (ocultar_linhas)
		return;
	linha_d->defPos({
			retangulo.x + retangulo.w,
			retangulo.y,
			retangulo.x + retangulo.w,
			retangulo.y + retangulo.h
		});
	linha_e->defPos({
			retangulo.x + 1,
			retangulo.y,
			retangulo.x,
			retangulo.y + retangulo.h
		});
	linha_c->defPos({
			retangulo.x,
			retangulo.y - 1,
			retangulo.x + retangulo.w,
			retangulo.y
		});
	linha_b->defPos({
			retangulo.x,
			retangulo.y + retangulo.h,
			retangulo.x + retangulo.w,
			retangulo.y + retangulo.h
		});
	linha_b->atualizar();
	linha_c->atualizar();
	linha_d->atualizar();
	linha_e->atualizar();
	posAtualizacao();
}

void BubbleUI::Formas::Moldura::renderizar(GLenum mode)
{
	Rect::renderizar(mode);
	if (ocultar_linhas)
		return;
	linha_c->renderizar();
	linha_e->renderizar();
	linha_d->renderizar();
	linha_b->renderizar();
}

BubbleUI::Contexto* BubbleUI::Formas::Moldura::obtCtx() const
{
	return contexto;
}