/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "depurador.hpp"
#include "src/ui/widgets/texto.hpp"
#include "src/depuracao/debug.hpp"

BubbleUI::Paineis::Depurador::
Depurador(const Vector4<int>& retangulo)
{
	this->retangulo = retangulo;
	Nome = "Depurador";
}

void BubbleUI::Paineis::Depurador::preAtualizacao()
{
	frames++;
	elapsedTime = glfwGetTime() - lastTime;
	// Atualizar FPS a cada segundo
	if (elapsedTime >= 1.0f) {
		//for (auto& str : Debug::obterMensagems())
		//{
		//	adicionarWidget(std::make_shared<Widgets::Texto>(str));
		//}

		fps_display->definirTexto("FPS: " + std::to_string(static_cast<int>(fps)));
		fps = frames / elapsedTime;
		frames = 0;  // Resetar o contador de frames
		lastTime = glfwGetTime();  // Atualizar o tempo de referência
	}
}

void BubbleUI::Paineis::Depurador::definirContexto(std::shared_ptr<Contexto> ctx)
{
	Painel::definirContexto(ctx);
	adicionarWidget(std::make_shared<Widgets::Texto>("Nome da GPU: " + contexto->NomeGpu));
	adicionarWidget(std::make_shared<Widgets::Texto>("Vercao Opengl: " + contexto->VercaoOpengl));
	fps_display = std::make_shared < Widgets::Texto>("FPS");
	adicionarWidget(fps_display);
}