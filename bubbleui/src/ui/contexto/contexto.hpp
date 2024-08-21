#pragma once
#include "src/util/includes.hpp"
#include "src/arquivadores/shader.hpp"
#include "src/inputs/inputs.hpp"
#include "bubbleui.hpp"
#include <glfw/glfw3.h>

namespace BubbleUI
{
    // Cont�m informa��es gerais para os paineis e widgets
    struct BEUI_DLL_API Contexto
    {
        GLFWwindow* glfwWindow{ nullptr }; // Janela glfw do contexto em quest�o
        GLFWcursor* cursor_horizontal{ nullptr };   // Cursor glfw
        GLFWcursor* cursor_vertical{ nullptr };     // Cursor glfw
        GLFWcursor* cursor_normal{ nullptr };       // Cursor glfw
        GLFWcursor* cursor{ nullptr }; // Cursor � ser usado
        Size tamanho; // Tamanho da janela glfw
        Bubble::Inputs::Inputs* inputs{ nullptr }; // Inputs da janela glfw

        Contexto();

        // Destructor to manage resources
        ~Contexto();
    };
}
