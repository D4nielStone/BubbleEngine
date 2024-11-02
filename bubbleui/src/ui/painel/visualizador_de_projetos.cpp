#include "visualizador_de_projetos.hpp"
#include "src/ui/widgets/botao.hpp"
#include <filesystem>
#include "src/ui/widgets/caixa_de_texto.hpp"
#include "src/ui/widgets/filtro.hpp"

BubbleUI::Paineis::VisualizadorDeProjetos::VisualizadorDeProjetos(std::shared_ptr<Contexto> contexto, const bool& preencher)
	: preencher(preencher)
{
	Nome = "Visualizador de Projetos";
	mostrar_aba = false;
	moldura.defCor({});
	configurar(contexto, {0, 0});
	if (preencher)
	{
		widgetPadding = {5, 5};
		selecionado = true;
		redimensionavel = false;
	}
	auto banner = std::make_shared<BubbleUI::Widgets::Imagem>("assets/texturas/icons/banner.png", 20);
	banner->quebrarLinha = true;
	adicionarWidget(banner);
	adicionarWidget(std::make_shared<BubbleUI::Widgets::Filtro>("Filtrar Projetos"));
}

void BubbleUI::Paineis::VisualizadorDeProjetos::posAtualizacao()
{
	definirTamanho({contexto->tamanho.width - static_cast<int>(retangulo.x), contexto->tamanho.height - static_cast<int>(retangulo.y) });
}
