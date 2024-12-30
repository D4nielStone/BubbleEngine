#pragma once
#include <iostream>
#include <unordered_map>
#include <functional>
#include "src/util/vetor2.hpp"
#include "src/util/vetor4.hpp"
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
        void keyPressed  (const char &key);
        void keyReleased (const char &key);
        bool isKeyPressed(const char &key) const;
        inputMode getInputMode() const;
        double mousex, mousey;
        int mouseEnter{ 0 }, teclado_action{ 0 }, mods{ 0 }, mouseButton{ 0 };
        char letra{ '\0' };
        bool mouseClick{ false }, char_press{ false };
    protected:
        virtual void handleKey(const char &key);
    private:
        inputMode currentMode{ inputMode::Editor };
        std::unordered_map<char, bool> keyStates;
    };

    // Callback de teclado GLFW
    extern "C" void mousePosCallBack(GLFWwindow* window, double x, double y);
    extern "C" void callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    extern "C" void mouseButtonCallBack(GLFWwindow* window, int a, int b, int c);
    extern "C" void charCallback(GLFWwindow* window, unsigned int codepoint);
}