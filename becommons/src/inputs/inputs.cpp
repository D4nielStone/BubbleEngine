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
static Key glfwKeyToKey(int glfwKey) {
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
        {GLFW_KEY_RIGHT_ALT, Key::Alt},
        {GLFW_KEY_BACKSPACE, Key::BS},
        {GLFW_KEY_ENTER, Key::ENTER},
        {GLFW_KEY_KP_ENTER, Key::ENTER},
        {GLFW_KEY_DELETE, Key::Del},
        {GLFW_KEY_F5, Key::F5}
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
         inputs->mouseButton = button;
    }
    else {
        std::cerr << "Erro: Ponteiro de usuário GLFW não está definido." << std::endl;
    }
}

// Callback para caracteres
void charCallback(GLFWwindow* window, unsigned int codepoint)
{
    Inputs* inputs = static_cast<Inputs*>(glfwGetWindowUserPointer(window));

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
    if (inputs->mods & GLFW_MOD_SHIFT && utf8_char.length() == 1) {
        utf8_char[0] = toupper(utf8_char[0]);
    }

    inputs->letra = utf8_char.empty() ? ' ' : utf8_char[0];
    inputs->char_press = true;
}
