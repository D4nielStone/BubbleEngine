#include "barra_menu.hpp"
#include "src/ui/formas/linha.hpp"
#include "pop_up.hpp"

BubbleUI::Util::BarraMenu::BarraMenu(std::shared_ptr<Contexto> contexto)
{
	this->contexto = contexto;
	cor_base = {0.1, 0.1, 0.1, 1};
	linha_d = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_b = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_e = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_c = std::make_unique<Formas::Linha>(Vector4{ 0, 0, 0, 0 }, contexto);
	linha_d->defCor({ 0.35f, 0.35f, 0.35f, 1.f });
	linha_b->defCor({ 0.35f, 0.35f, 0.35f, 1.f });
	linha_e->defCor({ 0.55f, 0.55f, 0.55f, 1.f });
	linha_c->defCor({ 0.55f, 0.55f, 0.55f, 1.f });

	popupCena = std::make_shared<PopUp>(contexto);

	arvoreCena = std::make_unique<Items::ItemMenu>("cena atual");
	arvoreCena->defMoldura(this);
}

void BubbleUI::Util::BarraMenu::renderizar() const
{
	Moldura::renderizar();
	arvoreCena->renderizar();
}

void BubbleUI::Util::BarraMenu::atualizar()
{
	retangulo = { 0, 1, contexto->tamanho.width, altura };
	posicaoWidget = { 0, 1 };
	widgetPadding = { 3, 3 };
	arvoreCena->atualizar();
	Moldura::atualizar();
}
