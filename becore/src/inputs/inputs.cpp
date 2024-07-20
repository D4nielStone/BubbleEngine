#include "inputs.h"
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

InputMode Inputs::getInputMode() const
{
    return currentMode;
}

Inputs::Inputs() : currentMode(InputMode::Editor) {
    for (int i = 0; i < static_cast<int>(Key::Count); ++i) {
        keyStates[static_cast<Key>(i)] = false;
    }
}

void Inputs::setInputMode(InputMode mode) {
    currentMode = mode;
}


void Inputs::keyPressed(Key key) {
    // Verifique se a chave é válida
    if (key < Key::W || key >= Key::Count) {
        std::cerr << "Tecla inválida pressionada: " << static_cast<int>(key) << std::endl;
        return;
    }

    // Verifique o estado do mapa antes de acessar
    if (keyStates.empty()) {
        std::cerr << "Erro: O mapa keyStates está vazio." << std::endl;
        return;
    }

    // Verifique se a chave existe no mapa
    auto it = keyStates.find(key);
    if (it != keyStates.end()) {
        keyStates[key] = true;
        handleKey(key);
    }
    else {
        std::cerr << "Tecla desconhecida pressionada: " << static_cast<int>(key) << std::endl;
    }
}


void Inputs::keyReleased(Key key) {

    auto it = keyStates.find(key);
    if (it != keyStates.end()) {
        keyStates[key] = false;
    }
    else {
        // Opcional: lidar com teclas desconhecidas
        std::cerr << "Tecla desconhecida liberada: " << static_cast<int>(key) << std::endl;
    }
}

bool Inputs::isKeyPressed(Key key) {
    auto it = keyStates.find(key);
    if (it != keyStates.end()) {
        return it->second;
    }
    return false; // Se a chave não estiver no mapa, retornamos false
}

void Inputs::handleKey(Key key) {
    if (currentMode == Game) {
        handleGameKey(key);
    }
    else {
        handleEditorKey(key);
    }
}

void Inputs::handleGameKey(Key key) {
    if (isKeyPressed(Key::Shift)) {
        std::cout << "Handling game key with Shift: " << static_cast<int>(key) << std::endl;
    }
    else if (isKeyPressed(Key::Ctrl)) {
        std::cout << "Handling game key with Ctrl: " << static_cast<int>(key) << std::endl;
    }
    else if (isKeyPressed(Key::Alt)) {
        std::cout << "Handling game key with Alt: " << static_cast<int>(key) << std::endl;
    }
    else {
        std::cout << "Handling game key: " << static_cast<int>(key) << std::endl;
    }
}

void Inputs::handleEditorKey(Key key) {
    if (isKeyPressed(Key::Shift)) {
        std::cout << "Handling InputMode::Editor key with Shift: " << static_cast<int>(key) << std::endl;
    }
    else if (isKeyPressed(Key::Ctrl)) {
        std::cout << "Handling InputMode::Editor key with Ctrl: " << static_cast<int>(key) << std::endl;
    }
    else if (isKeyPressed(Key::Alt)) {
        std::cout << "Handling InputMode::Editor key with Alt: " << static_cast<int>(key) << std::endl;
    }
    else {
        std::cout << "Handling InputMode::Editor key: " << static_cast<int>(key) << std::endl;
    }
}

Key glfwKeyToKey(int glfwKey) {
    switch (glfwKey) {
    case GLFW_KEY_W: return Key::W;
    case GLFW_KEY_A: return Key::A;
    case GLFW_KEY_S: return Key::S;
    case GLFW_KEY_D: return Key::D;
    case GLFW_KEY_E: return Key::E;
    case GLFW_KEY_Q: return Key::Q;
    case GLFW_KEY_RIGHT: return Key::RIGHT;
    case GLFW_KEY_LEFT: return Key::LEFT;
    case GLFW_KEY_DOWN: return Key::DOWN;
    case GLFW_KEY_UP: return Key::UP;
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_RIGHT_SHIFT: return Key::Shift;
    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_RIGHT_CONTROL: return Key::Ctrl;
    case GLFW_KEY_LEFT_ALT:
    case GLFW_KEY_RIGHT_ALT: return Key::Alt;
    default: return Key::Count; // Key::Count usado como valor inválido
    }
}

// Callback de teclado GLFW
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Inputs* inputs = static_cast<Inputs*>(glfwGetWindowUserPointer(window));

    if (inputs) {
        Key mappedKey = glfwKeyToKey(key);
        if (mappedKey != Key::Count) {
            if (action == GLFW_PRESS) {
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
