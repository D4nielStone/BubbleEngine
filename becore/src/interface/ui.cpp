#include "ui.hpp"
#include "src/nucleo/gerenciador.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ShlObj.h"
#include "src/nucleo/scenemanager.h"
#include "iostream"
#include "string"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "filesystem"

void Bubble::Interface::UI::limpar()
{ 
	layouts.clear();
}

std::wstring Bubble::Interface::UI::desktopPath()
{
	return std::wstring();
}
// PUBLICO
void Bubble::Interface::UI::pollevents()
{
	for (Layout* lay : layouts)
	{
		lay->atualizar();
	}
}
void Bubble::Interface::UI::inicializar(Nucleo::Gerenciador* gen, GLFWwindow* win)
{
	if (gen)
		gerenciador = gen;
	else
	{
		Debug::emitir("INTERFACE", "pobteiro so Gerenciador Nulo");
		return;
	}
	novaJanela(TipoLayout::L_MENU);
	janela_editor = novaJanela(TipoLayout::L_JANELA);
	auto img = Bubble::Interface::Imagem(gerenciador->engineAtual->obterGC()->cenaAtual()->camera_editor.textureColorbuffer);
	janela_editor->adicImagem(img);
}

void Bubble::Interface::UI::renderizar()
{
	glDisable(GL_DEPTH_TEST);
	for (Layout* lay : layouts)
	{
		lay->renderizar();
	}
	glEnable(GL_DEPTH_TEST);
}

void Bubble::Interface::UI::contextoAtual(GLFWwindow* w, Estilo)
{
}

Bubble::Interface::Layout* Bubble::Interface::UI::novaJanela(TipoLayout j)
{
	Layout* lay = new Layout(j);

	lay->defInputs(&inputs);
	layouts.push_back(lay);
	return lay;
}
