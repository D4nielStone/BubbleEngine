/** @copyright Copyright (c) 2024 Daniel Oliveira */

/// Definindo título da janela
constexpr const char* title = "Bubble Engine - Editor - (C) 2024 Daniel Oliveira";
#include "os/janela.hpp"
#include "src/entidades/entidade.hpp"
#include "src/componentes/propriedades.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/componentes/camera.hpp"

int main()
{
	Janela janela(title);
	GerenciadorDeEntidades ge;

	Entidade cubo = ge.criarEntidade();
	Entidade player = ge.criarEntidade();

	ge.adicionarComponente<Camera>(player, Vetor3(0.f, 0.f, -10.F),Vetor3(0.f, 0.f, 0.F), 75.f, 1.4F, 0.1F, 400.F);
	ge.adicionarComponente<Renderizador>(cubo, new Modelo(R"(C:\Users\DN\Downloads\survival_guitar_backpack\scene.gltf)"));

	janela.adicionarSistema<SistemaDeRenderizacao>(&ge);

	janela.iniciarLoop();

	return 0;
}