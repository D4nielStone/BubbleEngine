#include "contexto.hpp"
BubbleUI::Contexto* contexto_padrao;
BubbleUI::Contexto::Contexto()
{
	cursor_horizontal = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
	cursor_vertical = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
	cursor_normal = glfwCreateStandardCursor(GLFW_CURSOR_NORMAL);
	cursor = cursor_normal;
}

BubbleUI::Contexto::~Contexto()
{
	delete cursor_horizontal;
	delete cursor_vertical;
	delete cursor_normal;
}