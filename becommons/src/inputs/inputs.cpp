#include "inputs.hpp"
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <src/depuracao/debug.hpp>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

using namespace Bubble::Inputs;

InputMode Inputs::getInputMode() const {
    return currentMode;
}
Inputs::Inputs() : currentMode(InputMode::Editor) {
    // Inicializa o mapa com valores padrão
    for (int i = 0; i < static_cast<int>(Key::Count); ++i) {
        keyStates[static_cast<Key>(i)] = false;
    }
}
void Inputs::setInputMode(InputMode mode) {
    currentMode = mode;
}
void Inputs::keyPressed(Key key) {
    // Acesso direto ao mapa sem verificar se está vazio
    auto it = keyStates.find(key);
    if (it != keyStates.end()) {
        it->second = true;
        handleKey(key);
    }
    else {
        // Tecla inválida, pode ser registrado para depuração se necessário
        std::cerr << "Tecla desconhecida pressionada: " << static_cast<int>(key) << std::endl;
    }
}
void Inputs::keyReleased(Key key) {
    auto it = keyStates.find(key);
    if (it != keyStates.end()) {
        it->second = false;
    }
    else {
        // Tecla desconhecida, pode ser registrado para depuração se necessário
        std::cerr << "Tecla desconhecida liberada: " << static_cast<int>(key) << std::endl;
    }
}
bool Inputs::isKeyPressed(Key key) const
{
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}
void Inputs::handleKey(Key key) {
    switch (currentMode) {
    case Game:
        break;
    case Editor:
        break;
    default:
        std::cerr << "Modo de entrada desconhecido." << std::endl;
        break;
    }
}
Key glfwKeyToKey(int glfwKey) {
    static const std::unordered_map<int, Key> keyMap = {
        {GLFW_KEY_W, Key::W},
        {GLFW_KEY_A, Key::A},
        {GLFW_KEY_S, Key::S},
        {GLFW_KEY_D, Key::D},
        {GLFW_KEY_E, Key::E},
        {GLFW_KEY_Q, Key::Q},
        {GLFW_KEY_RIGHT, Key::RIGHT},
        {GLFW_KEY_LEFT, Key::LEFT},
        {GLFW_KEY_DOWN, Key::DOWN},
        {GLFW_KEY_UP, Key::UP},
        {GLFW_KEY_LEFT_SHIFT, Key::Shift},
        {GLFW_KEY_RIGHT_SHIFT, Key::Shift},
        {GLFW_KEY_LEFT_CONTROL, Key::Ctrl},
        {GLFW_KEY_RIGHT_CONTROL, Key::Ctrl},
        {GLFW_KEY_LEFT_ALT, Key::Alt},
        {GLFW_KEY_RIGHT_ALT, Key::Alt}
    };

    auto it = keyMap.find(glfwKey);
    return it != keyMap.end() ? it->second : Key::Count;
}
// Callback de teclado GLFW
void callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    Inputs* inputs = static_cast<Inputs*>(glfwGetWindowUserPointer(window));
    inputs->mods = mods;
    inputs->teclado_action = action;

    if (inputs) {
        Key mappedKey = glfwKeyToKey(key);
        if (mappedKey != Key::Count) {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                inputs->keyPressed(mappedKey);
            }
            else if (action == GLFW_RELEASE) {
                inputs->keyReleased(mappedKey);
            }
        }
    }
    else {
        std::cerr << "Erro: Ponteiro de usuário GLFW não está definido." << std::endl;
    }
    // Captura caracteres imprimíveis
    if ((key >= GLFW_KEY_A && key <= GLFW_KEY_Z) || (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) || key == GLFW_KEY_SPACE)
    {
        inputs->letra = static_cast<char>(key);

        // Ajusta para letras minúsculas se a tecla SHIFT não estiver pressionada
        if (mods & GLFW_MOD_SHIFT)
        {
            inputs->letra = toupper(inputs->letra);
            inputs->mods = 0;
        }
        else
        {
            inputs->letra = tolower(inputs->letra);
        }

    }
    else if (key == GLFW_KEY_BACKSPACE)
    {
        inputs->letra = '\b';
    }

}
// Callback de posição do mouse
void mousePosCallBack(GLFWwindow* window, double x, double y) 
{
    Inputs* inputs = static_cast<Inputs*>(glfwGetWindowUserPointer(window));

    if (inputs) {
        inputs->mousex = x;
        inputs->mousey = y;
    }
    else {
        std::cerr << "Erro: Ponteiro de usuário GLFW não está definido." << std::endl;
    }
}
// Callback de clique do mouse
void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) 
{
    Inputs* inputs = static_cast<Inputs*>(glfwGetWindowUserPointer(window));

    if (inputs) {
        // Processar o clique do mouse
         inputs->mouseEnter = action;
    }
    else {
        std::cerr << "Erro: Ponteiro de usuário GLFW não está definido." << std::endl;
    }
}
