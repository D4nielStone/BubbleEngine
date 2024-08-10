#pragma once
#include "src/util/includes.hpp"
#include <src/arquivadores/shader.hpp>
#include "src/inputs/inputs.hpp"
#include "bubbleui.hpp"
#include "glfw/glfw3.h"

namespace BubbleUI
{
    struct BEUI_DLL_API Contexto
    {
        GLFWwindow* glfwWindow;
        Size tamanho;
        Size tamanho_ini;
        Shader shader;
        GLFWcursor* cursor;
        Bubble::Inputs::Inputs* inputs;
        Contexto();
    };
}