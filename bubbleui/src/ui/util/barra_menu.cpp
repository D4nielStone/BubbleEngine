#include "barra_menu.hpp"
#include "src/ui/formas/linha.hpp"

BubbleUI::Util::BarraMenu::BarraMenu(std::shared_ptr<Contexto> contexto)
{
	this->contexto = contexto;
	linha_d = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_b = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_e = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_c = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_d->defCor({ 0.35, 0.35, 0.35 });
	linha_b->defCor({ 0.35, 0.35, 0.35 });
	linha_e->defCor({ 0.55, 0.55, 0.55 });
	linha_c->defCor({ 0.55, 0.55, 0.55 });
}

void BubbleUI::Util::BarraMenu::renderizar() const
{
	Moldura::renderizar();
}

void BubbleUI::Util::BarraMenu::atualizar()
{
	retangulo.w = contexto->tamanho.width; // ajusta para não ser afetado pelo contexto
	retangulo.h = altura;
	Moldura::atualizar();
}
