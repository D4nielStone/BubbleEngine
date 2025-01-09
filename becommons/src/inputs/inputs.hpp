#pragma once
#include <iostream>
#include <unordered_map>
#include <functional>
#include "src/util/vetor2.hpp"
#include "src/util/vetor4.hpp"
#include <glm/glm.hpp>
#include "becommons.hpp"
struct GLFWwindow;

namespace bubble
{
    enum inputMode {
        Editor,
        Game,
        Default
    };

    class inputs {
    public:
        inputs();
        void setInputMode(inputMode mode);
        void keyPressed  (const std::string &key);
        void keyReleased (const std::string &key);
        bool isKeyPressed(const std::string &key) const;
        inputMode getInputMode() const;
        double mousex, mousey;
        int mouseEnter{ 0 }, teclado_action{ 0 }, mods{ 0 }, mouseButton{ 0 };
        char letra{ '\0' };
        bool mouseClick{ false }, char_press{ false };
        std::unordered_map<std::string, bool> keyStates;
    protected:
        virtual void handleKey(const std::string&key);
    private:
        inputMode currentMode{ inputMode::Editor };
    };

    // Callback de teclado GLFW
    extern "C" void mousePosCallBack(GLFWwindow* window, double x, double y);
    extern "C" void callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    extern "C" void mouseButtonCallBack(GLFWwindow* window, int a, int b, int c);
    extern "C" void charCallback(GLFWwindow* window, unsigned int codepoint);
    bubble::vetor2<double> obterMouse();
    bubble::vetor2<int> tamanhoJanela();
    bool pressionada(const std::string &tecla);
}