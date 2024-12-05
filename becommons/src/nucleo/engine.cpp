/** @copyright Copyright (c) 2024 Daniel Oliveira */
#include "engine.hpp"
using namespace Bubble;

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
	Debug::emitir("Engine", "carregando projeto em: " + path);
	gerenciador_dc->novaCena("Cena", false);
	Bubble::Cena::criarEntidade(R"(C:\Users\DN\3D Objects\fence-barricade-03\source\Fence-Barricade_03.blend)");;
	return true;
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