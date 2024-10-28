#include "visualizador_de_projetos.hpp"
#include "src/ui/widgets/botao.hpp"
#include <filesystem>

BubbleUI::Paineis::VisualizadorDeProjetos::VisualizadorDeProjetos(std::shared_ptr<Contexto> contexto, const bool& preencher)
	: preencher(preencher)
{
	Nome = "Visualizador de Projetos";
	configurar(contexto, {0, 29});
	if (preencher)
	{
		widgetPadding = {1,1};
		selecionado = true;
		redimensionavel = false;
	}
	recarregar();
}

void BubbleUI::Paineis::VisualizadorDeProjetos::posAtualizacao()
{
	definirTamanho({contexto->tamanho.width, contexto->tamanho.height});
}

void Paineis::VisualizadorDeProjetos::recarregar()
{
	// itera sobre as pastas filho
	try
	{
		for (const auto& pasta : std::filesystem::directory_iterator(contexto->dirDoProjeto))
		{
			// itera para identificar icone
			std::string path_icone{"ICON.ico"};
			for (const auto& entry : std::filesystem::directory_iterator(pasta.path())) 
			{
				if (std::filesystem::is_regular_file(entry.path()) && entry.path().extension() == ".ico") {
					std::cout << "Arquivo encontrado: " << entry.path() << '\n';
					path_icone = entry.path().string();
					break; // Encerra a busca ao encontrar o arquivo
				}
			}
			diretorios_projetos[pasta.path().filename().string()] = path_icone;
		}
	}
	catch(std::filesystem::filesystem_error& error)
	{
		std::cerr << "Erro de diretório com visualizador de projetos: " << error.what() << "\n";
	}
	for (const auto& projeto : diretorios_projetos)
	{
		adicionarWidget(std::make_shared<Widgets::Botao>(projeto.first, projeto.second));
	}
}