#include "moldura.hpp"

BubbleUI::Formas::Moldura::Moldura(Contexto* contexto, Vector4 retangulo)
{
	this->contexto = contexto;
	this->retangulo = retangulo;
	linha_d = new Linha({0, 0, 0, 0}, contexto);
	linha_b = new Linha({0, 0, 0, 0}, contexto);
	linha_e = new Linha({0, 0, 0, 0}, contexto);
	linha_c = new Linha({0, 0, 0, 0}, contexto);
}

void BubbleUI::Formas::Moldura::atualizar(float deltaTime)
{
	linha_d->defPos({
			retangulo.x + retangulo.w,
			retangulo.y - 1.f,
			0,
			(float)retangulo.h
		});
	linha_e->defPos({
			retangulo.x,
			retangulo.y - 1.f,
			0,
			(float)retangulo.h
		});
	linha_c->defPos({
			retangulo.x,
			retangulo.y,
			(float)retangulo.w,
			0
		});
	linha_b->defPos({
			retangulo.x,
			retangulo.y + retangulo.h,
			(float)retangulo.w,
			0
		});
	linha_b->atualizar(deltaTime);
	linha_c->atualizar(deltaTime);
	linha_d->atualizar(deltaTime);
	linha_e->atualizar(deltaTime);
	Rect::atualizar(deltaTime);
}

void BubbleUI::Formas::Moldura::renderizar(GLenum)
{
	Rect::renderizar(GL_TRIANGLES);
	linha_b->renderizar();
	linha_c->renderizar();
	linha_d->renderizar();
	linha_e->renderizar();
}
