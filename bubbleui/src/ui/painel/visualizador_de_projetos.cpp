
// Copyright (c) 2024 Daniel Oliveira

#include "visualizador_de_projetos.hpp"
#include "src/ui/widgets/botao.hpp"
#include <filesystem>
#include "src/ui/widgets/caixa_de_texto.hpp"
#include "src/ui/widgets/filtro.hpp"

using namespace BubbleUI::Widgets;

BubbleUI::Paineis::VisualizadorDeProjetos::VisualizadorDeProjetos(const bool& preencher, const Vector4& retangulo)
	: preencher(preencher)
{
	tamanhoMinimo = {300, 500};
	Nome = "Visualizador de Projetos";
	widgetPadding = { 5, 5 };
	this->retangulo = retangulo;
	if (preencher)
	{
		mostrar_aba = false;
		selecionado = true;
		redimensionavel = false;
	}
}

void BubbleUI::Paineis::VisualizadorDeProjetos::posAtualizacao()
{
	if (preencher)
	{
		definirPosicao({ 0, 0 });
		definirTamanho({ contexto->tamanho.width - static_cast<int>(obterRetangulo().x), contexto->tamanho.height - static_cast<int>(obterRetangulo().y) });
	}
}
void  BubbleUI::Paineis::VisualizadorDeProjetos::definirContexto(std::shared_ptr<Contexto> ctx)
{
	Painel::definirContexto(ctx);

	auto banner = std::make_shared<Imagem>("assets/texturas/icons/banner.png", 20);
	banner->quebrarLinha = true;
	adicionarWidget(banner);
	auto filtro = std::make_shared<Filtro>("Filtrar Projetos");
	adicionarWidget(filtro);
	filtro->recarregar();
}