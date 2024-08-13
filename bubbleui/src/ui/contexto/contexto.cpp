#include "contexto.hpp"
BubbleUI::Contexto* contexto_padrao;
BubbleUI::Contexto::Contexto()
{
	cursor_horizontal = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
	cursor_vertical = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
	cursor_normal = glfwCreateStandardCursor(GLFW_CURSOR_NORMAL);
	cursor = cursor_normal;
	shader = Shader("assets/shaders/layout.vert", "assets/shaders/layout.frag");
}

BubbleUI::Contexto* BubbleUI::obtContextoPadrao()
{
	return contexto_padrao;
}

void BubbleUI::defContextoPadrao(Contexto* ctx)
{
	contexto_padrao = ctx;
}