#include "ui.hpp"
#include "layout/layout.hpp"
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

// PRIVADO
void Bubble::Interface::UI::desenharMenuArquivos()
{
}

void Bubble::Interface::UI::desenharMenuCena()
{
}

void Bubble::Interface::UI::desenharMenuEditar()
{
}

void Bubble::Interface::UI::desenharMenuCriarP()
{
}

void Bubble::Interface::UI::desenharProjetos(unsigned int depth)
{
}

void Bubble::Interface::UI::desenharEditor(unsigned int depth)
{
}

void Bubble::Interface::UI::desenharPreview(unsigned int depth)
{
}

void Bubble::Interface::UI::desenharConsole(unsigned int depth)
{
}

void Bubble::Interface::UI::desenharInspetor(unsigned int depth)
{
}

void Bubble::Interface::UI::desenharArquivos(unsigned int depth)
{
}

void Bubble::Interface::UI::desenharEntidades(unsigned int depth)
{
}

void Bubble::Interface::UI::iniciarJanelas()
{
}

void Bubble::Interface::UI::desenharComponente(Comum::Componente* componente)
{
}

void Bubble::Interface::UI::limpar()
{ 
   
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
void Bubble::Interface::UI::inicializarImGui(Nucleo::Gerenciador* gen, GLFWwindow* win)
{
	if (gen)
		gerenciador = gen;
	else
	{
		Debug::emitir("INTERFACE", "pobteiro so Gerenciador Nulo");
		return;
	}
	novaJanela(Janela::MENU_CriarProjeto);
}

void Bubble::Interface::UI::renderizar()
{
	for (Layout* lay : layouts)
	{
		lay->renderizar();
	}
}

void Bubble::Interface::UI::contextoAtual(GLFWwindow* w, Estilo)
{

}

void Bubble::Interface::UI::novaJanela(Janela j)
{
	Layout* lay = new Layout(j);
	Quadrado q(Vector2(0,0), Vector2(0.2,0.2));
	q.defCor(Color(1,1,1));
	lay->adicItem(q);
	lay->defInputs(&inputs);
	layouts.push_back(lay);
}
