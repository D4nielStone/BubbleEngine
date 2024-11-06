#include "visualizador_de_projetos.hpp"
#include "src/ui/widgets/botao.hpp"
#include <filesystem>
#include "src/ui/widgets/caixa_de_texto.hpp"
#include "src/ui/widgets/filtro.hpp"

using namespace BubbleUI::Widgets;

BubbleUI::Paineis::VisualizadorDeProjetos::VisualizadorDeProjetos(std::shared_ptr<Contexto> contexto, const bool& preencher)
	: preencher(preencher)
{
	Nome = "Visualizador de Projetos";
	mostrar_aba = false;
	configurar(contexto, {0, 0});
	if (preencher)
	{
		widgetPadding = {5, 5};
		selecionado = true;
		redimensionavel = false;
	}
	auto banner = std::make_shared<Imagem>("assets/texturas/icons/banner.png", 20);
	banner->quebrarLinha = true;
	adicionarWidget(banner);
	auto filtro = std::make_shared<Filtro>("Filtrar Projetos");
	adicionarWidget(filtro);
	filtro->recarregar();
}

void BubbleUI::Paineis::VisualizadorDeProjetos::posAtualizacao()
{
	definirPosicao({0, 0});
	definirTamanho({contexto->tamanho.width - static_cast<int>(obterRetangulo().x), contexto->tamanho.height - static_cast<int>(obterRetangulo().y)});
}
