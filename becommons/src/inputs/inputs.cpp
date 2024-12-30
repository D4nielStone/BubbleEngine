#include "inputs.hpp"
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <src/depuracao/debug.hpp>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <os/janela.hpp>

using namespace bubble;
static const std::unordered_map<int, char> keyMap = {
        {GLFW_KEY_W, 'W'},
        {GLFW_KEY_A, 'A'},
        {GLFW_KEY_S, 'S'},
        {GLFW_KEY_D, 'D'},
        {GLFW_KEY_E, 'E'},
        {GLFW_KEY_Q, 'Q'},
        {GLFW_KEY_RIGHT, 'Dire'},
        {GLFW_KEY_LEFT, 'Esqu'},
        {GLFW_KEY_DOWN, 'Baix'},
        {GLFW_KEY_UP, 'Cima'},
        {GLFW_KEY_LEFT_SHIFT, 'Shif'},
        {GLFW_KEY_RIGHT_SHIFT, 'Shif'},
        {GLFW_KEY_LEFT_CONTROL, 'Ctrl'},
        {GLFW_KEY_RIGHT_CONTROL, 'Ctrl'},
        {GLFW_KEY_LEFT_ALT, 'Alt'},
        {GLFW_KEY_RIGHT_ALT, 'Alt'},
        {GLFW_KEY_BACKSPACE, 'BS'},
        {GLFW_KEY_ENTER, 'Ente'},
        {GLFW_KEY_KP_ENTER, 'Ente'},
        {GLFW_KEY_DELETE, 'Del'},
        {GLFW_KEY_F5, 'F5'}
};

inputMode inputs::getInputMode() const {
    return currentMode;
}
inputs::inputs() : currentMode(inputMode::Game) {
    // Inicializa o mapa com valores padrão
    for (auto& key : keyMap)
    {
        keyStates.insert(std::pair(key.second, false));
    }
}
void inputs::setInputMode(inputMode mode) {
    currentMode = mode;
}
void inputs::keyPressed(const char &key) {
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
void inputs::keyReleased(const char &key) {
    auto it = keyStates.find(key);
    if (it != keyStates.end()) {
        it->second = false;
    }
    else {
        // Tecla desconhecida, pode ser registrado para depuração se necessário
        std::cerr << "Tecla desconhecida liberada: " << static_cast<int>(key) << std::endl;
    }
}
bool inputs::isKeyPressed(const char &key) const
{
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}
void inputs::handleKey(const char &key) {
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
static char glfwkeyTokey(int glfwkey) {

    auto it = keyMap.find(glfwkey);
    return it != keyMap.end() ? it->second : 'Erro';
}
// Callback de teclado GLFW
void bubble::callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    auto &input = static_cast<bubble::janela*>(glfwGetWindowUserPointer(window))->inputs;
    input.mods = mods;
    input.teclado_action = action;

        char mappedkey = glfwkeyTokey(key);
        if (mappedkey != 'Erro') {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                input.keyPressed(mappedkey);
            }
            else if (action == GLFW_RELEASE) {
                input.keyReleased(mappedkey);
            }
        }
}
// Callback de posição do mouse
void bubble::mousePosCallBack(GLFWwindow* window, double x, double y)
{
    auto& input = static_cast<bubble::janela*>(glfwGetWindowUserPointer(window))->inputs;

        input.mousex = x;
        input.mousey = y;
    
}
// Callback de clique do mouse
void bubble::mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
{
    auto& input = static_cast<bubble::janela*>(glfwGetWindowUserPointer(window))->inputs;

        // Processar o clique do mouse
         input.mouseEnter = action;
         input.mouseButton = button;
    
}

// Callback para caracteres
void bubble::charCallback(GLFWwindow* window, unsigned int codepoint)
{
    auto& input = static_cast<bubble::janela*>(glfwGetWindowUserPointer(window))->inputs;

    // Converte o código Unicode para um caractere UTF-8
    std::string utf8_char;
    if (codepoint <= 0x7F) {
        // Caracteres ASCII
        utf8_char.push_back(static_cast<char>(codepoint));
    }
    else if (codepoint <= 0x7FF) {
        // Caracteres UTF-8 com 2 bytes
        utf8_char.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
        utf8_char.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
    }
    else if (codepoint <= 0xFFFF) {
        // Caracteres UTF-8 com 3 bytes
        utf8_char.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
        utf8_char.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
        utf8_char.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
    }
    else if (codepoint <= 0x10FFFF) {
        // Caracteres UTF-8 com 4 bytes
        utf8_char.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
        utf8_char.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
        utf8_char.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
        utf8_char.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
    }

    // Ajusta para letras maiúsculas/minúsculas se a tecla SHIFT estiver pressionada
    if (input.mods & GLFW_MOD_SHIFT && utf8_char.length() == 1) {
        utf8_char[0] = toupper(utf8_char[0]);
    }

    input.letra = utf8_char.empty() ? ' ' : utf8_char[0];
    input.char_press = true;
}