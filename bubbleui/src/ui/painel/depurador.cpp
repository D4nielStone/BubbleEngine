#include "depurador.hpp"
#include "src/ui/widgets/texto.hpp"

BubbleUI::Paineis::Depurador::Depurador(Contexto* ctx) : fps_display("FPS: ")
{
	configurar(ctx);
	adiWidget(new Widgets::Texto(&fps_display, 12));
	adiWidget(new Widgets::Texto("\n", 12));
}

void BubbleUI::Paineis::Depurador::preAtualizacao()
{
	frames++;
	elapsedTime = glfwGetTime() - lastTime;
	// Atualizar FPS a cada segundo
	if (elapsedTime >= 1.0f) {
		fps = frames / elapsedTime;
		frames = 0;  // Resetar o contador de frames
		lastTime = glfwGetTime();  // Atualizar o tempo de refer�ncia
	}
	fps_display = "FPS: " + std::to_string(fps);
}