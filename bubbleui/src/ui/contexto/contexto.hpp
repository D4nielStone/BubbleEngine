#pragma once
#include "src/util/includes.h"
#include <src/arquivadores/shader.h>
#include "src/inputs/inputs.h"
#include "bubbleui.h"

struct GLFWwindow;
struct GLFWcursor;
namespace BubbleUI
{
    struct BEUI_DLL_API Contexto
    {
        GLFWwindow* glfwWindow;
        Size tamanho;
        Shader shader;
        GLFWcursor* cursor;
        Bubble::Inputs::Inputs* inputs;
        Contexto();
    };
}