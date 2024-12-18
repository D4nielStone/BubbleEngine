#include "inputs.hpp"
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <src/depuracao/debug.hpp>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

using namespace bubble;

inputMode inputs::getInputMode() const {
    return currentMode;
}
inputs::inputs() : currentMode(inputMode::Editor) {
    // Inicializa o mapa com valores padrão
    for (int i = 0; i < static_cast<int>(key::Count); ++i) {
        keyStates[static_cast<key>(i)] = false;
    }
}
void inputs::setInputMode(inputMode mode) {
    currentMode = mode;
}
void inputs::keyPressed(const key &key) {
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
void inputs::keyReleased(const key &key) {
    auto it = keyStates.find(key);
    if (it != keyStates.end()) {
        it->second = false;
    }
    else {
        // Tecla desconhecida, pode ser registrado para depuração se necessário
        std::cerr << "Tecla desconhecida liberada: " << static_cast<int>(key) << std::endl;
    }
}
bool inputs::isKeyPressed(const key &key) const
{
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}
void inputs::handleKey(const key &key) {
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
static key glfwkeyTokey(int glfwkey) {
    static const std::unordered_map<int, key> keyMap = {
        {GLFW_KEY_W, key::W},
        {GLFW_KEY_A, key::A},
        {GLFW_KEY_S, key::S},
        {GLFW_KEY_D, key::D},
        {GLFW_KEY_E, key::E},
        {GLFW_KEY_Q, key::Q},
        {GLFW_KEY_RIGHT, key::RIGHT},
        {GLFW_KEY_LEFT, key::LEFT},
        {GLFW_KEY_DOWN, key::DOWN},
        {GLFW_KEY_UP, key::UP},
        {GLFW_KEY_LEFT_SHIFT, key::Shift},
        {GLFW_KEY_RIGHT_SHIFT, key::Shift},
        {GLFW_KEY_LEFT_CONTROL, key::Ctrl},
        {GLFW_KEY_RIGHT_CONTROL, key::Ctrl},
        {GLFW_KEY_LEFT_ALT, key::Alt},
        {GLFW_KEY_RIGHT_ALT, key::Alt},
        {GLFW_KEY_BACKSPACE, key::BS},
        {GLFW_KEY_ENTER, key::ENTER},
        {GLFW_KEY_KP_ENTER, key::ENTER},
        {GLFW_KEY_DELETE, key::Del},
        {GLFW_KEY_F5, key::F5}
    };

    auto it = keyMap.find(glfwkey);
    return it != keyMap.end() ? it->second : key::Count;
}
// Callback de teclado GLFW
void bubble::callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    bubble::inputs* input = static_cast<bubble::inputs*>(glfwGetWindowUserPointer(window));
    input->mods = mods;
    input->teclado_action = action;

    if (input) {
        bubble::key mappedkey = glfwkeyTokey(key);
        if (mappedkey != key::Count) {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                input->keyPressed(mappedkey);
            }
            else if (action == GLFW_RELEASE) {
                input->keyReleased(mappedkey);
            }
        }
    }
    else {
        std::cerr << "Erro: Ponteiro de usuário GLFW não está definido." << std::endl;
    }
}
// Callback de posição do mouse
void bubble::mousePosCallBack(GLFWwindow* window, double x, double y)
{
    bubble::inputs* input = static_cast<bubble::inputs*>(glfwGetWindowUserPointer(window));

    if (input) {
        input->mousex = x;
        input->mousey = y;
    }
    else {
        std::cerr << "Erro: Ponteiro de usuário GLFW não está definido." << std::endl;
    }
}
// Callback de clique do mouse
void bubble::mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
{
    bubble::inputs* input = static_cast<bubble::inputs*>(glfwGetWindowUserPointer(window));

    if (input) {
        // Processar o clique do mouse
         input->mouseEnter = action;
         input->mouseButton = button;
    }
    else {
        std::cerr << "Erro: Ponteiro de usuário GLFW não está definido." << std::endl;
    }
}

// Callback para caracteres
void bubble::charCallback(GLFWwindow* window, unsigned int codepoint)
{
    bubble::inputs* inputs = static_cast<bubble::inputs*>(glfwGetWindowUserPointer(window));

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
