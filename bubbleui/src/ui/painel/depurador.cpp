// Copyright (c) 2024 Daniel Oliveira

#include "depurador.hpp"
#include "src/ui/widgets/texto.hpp"
#include "src/depuracao/debug.hpp"

BubbleUI::Paineis::Depurador::
Depurador(std::shared_ptr < Contexto> ctx, const Vector4& retangulo) : fps_display("FPS: ")
{
	Nome = "Depurador";
	configurar(ctx, retangulo);
	adicionarWidget(std::make_shared<Widgets::Texto>("Nome da GPU: " + contexto->NomeGpu));
	adicionarWidget(std::make_shared<Widgets::Texto>("Vercao Opengl: " + contexto->VercaoOpengl));
	adicionarWidget(std::make_shared<Widgets::Texto>(&fps_display));
}

void BubbleUI::Paineis::Depurador::preAtualizacao()
{
	frames++;
	elapsedTime = glfwGetTime() - lastTime;
	// Atualizar FPS a cada segundo
	if (elapsedTime >= 1.0f) {
		if (lista_widgets.size() > 3)
			lista_widgets.erase(lista_widgets.begin()+1, lista_widgets.end());
		for (auto& str : Debug::obterMensagems())
		{
			adicionarWidget(std::make_shared<Widgets::Texto>(str));
		}

		fps = frames / elapsedTime;
		frames = 0;  // Resetar o contador de frames
		lastTime = glfwGetTime();  // Atualizar o tempo de referência
		fps_display = "FPS: " + std::to_string(static_cast<int>(fps));
	}
}
