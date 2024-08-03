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
#include "src/depuracao/ASSERT.hpp"

void Bubble::Interface::UI::limpar()
{ 
	layouts.clear();
}

std::wstring Bubble::Interface::UI::desktopPath()
{
	return std::wstring();
}
void Bubble::Interface::UI::definirPanoDeFundo(std::string path)
{
	pano_de_fundo = new Imagem(path, 1.f); pano_de_fundo->defInputs(&inputs);
	pano_de_fundo->preencher();
}
// PUBLICO
void Bubble::Interface::UI::pollevents()
{
	pano_de_fundo->atualizar();
	for (Quadrado* quad : layouts)
	{
		quad->atualizar();
	}
}
void Bubble::Interface::UI::inicializar(Nucleo::Gerenciador* gen)
{
	ASSERT(gen != nullptr);
	gerenciador = gen;

	definirPanoDeFundo();

	novaJanela(TipoLayout::L_MENU);
	janela_editor = static_cast<Layout*>(novaJanela(TipoLayout::L_JANELA));
	Imagem buffer_editor(gerenciador->engineAtual->obterGC()->cenaAtual()->camera_editor.textureColorbuffer);
	buffer_editor.preencher();
	janela_editor->adicImagem(buffer_editor);
}

void Bubble::Interface::UI::renderizar()
{
	glDisable(GL_DEPTH_TEST);
	pano_de_fundo->renderizar();
	for (Quadrado* quad : layouts)
	{
		quad->renderizar();
	}
	glEnable(GL_DEPTH_TEST);
}

void Bubble::Interface::UI::contextoAtual(GLFWwindow* w, Estilo)
{
}

Bubble::Interface::Quadrado* Bubble::Interface::UI::novaJanela(TipoLayout j)
{
	Quadrado* quad;
	switch (j)
	{
	case Bubble::L_MENU:
		break;
	case Bubble::L_JANELA:
		quad = new Layout();
		quad->defInputs(&inputs);
		layouts.push_back(quad);
		return quad;
		break;
	default:
		break;
	}
}
