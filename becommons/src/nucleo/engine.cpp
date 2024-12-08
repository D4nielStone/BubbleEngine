/** @copyright Copyright (c) 2024 Daniel Oliveira */
#include "engine.hpp"
#include <filesystem>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
namespace Bubble
{
	Engine::Engine(const std::string& path) : gerenciador_dc(std::make_shared<Cena::SceneManager>()), diretorio_projeto(path)
	{
		Debug::emitir("Engine", "ola mundo!");
		Bubble::Cena::definirSceneManager(gerenciador_dc);
	}
	Engine::Engine() : gerenciador_dc(std::make_shared<Cena::SceneManager>())
	{
		Debug::emitir("Engine", "ola mundo!");
		Bubble::Cena::definirSceneManager(gerenciador_dc);
	}
	std::shared_ptr<Cena::SceneManager> Engine::obterGerenciadorDeCenas()
	{

		return gerenciador_dc;
	}
	
	bool Engine::carregarProjeto(const std::string& path)
	{
		diretorio_projeto = path;
		if(std::filesystem::exists(path))
		Debug::emitir("Engine", "Carregando projeto em: " + path);
		else {
			Debug::emitir("Engine", "Diretorio " + path + " nao encontrado, criando novo projeto");
			std::filesystem::create_directories(path);
		}

		std::vector<std::string> cenas; ///< Nome de cada Cena
		int cena_atual = 0;			///< Index cena atual
		/// Abre arquivo
		std::ifstream arquivo_config(diretorio_projeto + "\\config.bproj", std::ifstream::binary);
		if (arquivo_config.is_open())
		{


			// 1. Ler a cena atual
			arquivo_config.read(reinterpret_cast<char*>(&cena_atual), sizeof(cena_atual)); 
			if (!arquivo_config) {
				Debug::emitir("Engine", "Erro ao ler cena atual.");
				return false;
			}
			// 2. Ler o array de nomes de cenas
			size_t numero_de_cenas = 0;
			arquivo_config.read(reinterpret_cast<char*>(&numero_de_cenas), sizeof(numero_de_cenas));
			if (!arquivo_config) {
				Debug::emitir("Engine", "Erro ao ler o n�mero de cenas.");
				return false;
			}

			Debug::emitir("Engine", "Cenas do projeto:");
			for (size_t i = 0; i < numero_de_cenas; ++i) {
				size_t tamanho_cena;
				arquivo_config.read(reinterpret_cast<char*>(&tamanho_cena), sizeof(tamanho_cena));
				if (!arquivo_config) {
					Debug::emitir("Engine", "Erro ao ler o cenas.");
					return false;
				}

				std::string cena(tamanho_cena, '\0');
				arquivo_config.read(&cena[0], tamanho_cena);
				cenas.push_back(cena);
				Debug::emitir("Engine", cena);
			}
		}
		else
		{
			/// Caso n�o tem arquivo de configura��o
			Debug::emitir("Engine", "Erro ao abrir o arquivo de configura��o.");
			gerenciador_dc->novaCena("Cena", false);
		}
		arquivo_config.close();

		// Atualizar o gerenciador de cenas
		gerenciador_dc->carregarCenas(cenas); 
		if (cena_atual >= 0 && cena_atual < gerenciador_dc->obterCenas().size()) {
			gerenciador_dc->carregarCena(cena_atual);
		}
		else {
			Debug::emitir("Engine", "�ndice da cena atual inv�lido, carregando cena padr�o.");
			gerenciador_dc->carregarCena(0); // Carrega uma cena padr�o, se aplic�vel
		}

		Debug::emitir("Engine", "Projeto carregado com sucesso!");
		return true;
	}
	void Engine::salvarProjeto() {
		Debug::emitir("Engine", "Salvando projeto em: " + diretorio_projeto);

		// Criar diret�rio de cenas, se necess�rio
		std::filesystem::path cenasDir = diretorio_projeto + "\\Cenas";
		if (!std::filesystem::exists(cenasDir)) {
			std::filesystem::create_directories(cenasDir);
		}

		// Abrir arquivo bin�rio para salvar configura��o
		std::ofstream arquivo_config(diretorio_projeto + "\\config.bproj", std::ofstream::binary);
		if (!arquivo_config.is_open()) {
			Debug::emitir("Engine", "Erro ao criar o arquivo de configura��o.");
			return;
		}

		// Salvar a cena atual
		const int cena_atual = gerenciador_dc->cenaAtualIdx(); // Supondo que este m�todo existe
		arquivo_config.write(reinterpret_cast<const char*>(&cena_atual), sizeof(cena_atual));

		// 2. Salvar o array de nomes de cenas
		const auto& cenas = gerenciador_dc->obterCenas(); // Supondo que retorna um `std::vector<std::string>`
		const size_t numero_de_cenas = cenas.size();
		arquivo_config.write(reinterpret_cast<const char*>(&numero_de_cenas), sizeof(numero_de_cenas));
		for (const auto& cena : cenas) {
			const size_t tamanho_cena = cena->nome().size();
			arquivo_config.write(reinterpret_cast<const char*>(&tamanho_cena), sizeof(tamanho_cena));
			arquivo_config.write(cena->nome().c_str(), tamanho_cena);
		}

		arquivo_config.close();

		gerenciador_dc->salvarCenas();

		Debug::emitir("Engine", "Projeto salvo com sucesso!");
	}


	void Engine::atualizar()
	{
		gerenciador_dc->atualizarCenaAtual();
	}
	void Engine::renderizar()
	{
		gerenciador_dc->renderizarCenaAtual();
	}
	void Engine::definirInputs(std::shared_ptr<Inputs::Inputs> inputs)
	{
		Debug::emitir("Engine", "inputs definidos");
		gerenciador_dc->defIputs(inputs);
	}

	std::shared_ptr<Engine> Engine::instance = nullptr;
}