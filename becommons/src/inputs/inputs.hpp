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
    enum key : uint8_t {
        W,
        A,
        S,
        D,
        E,
        Q,
        UP,
        RIGHT,
        LEFT,
        DOWN,
        Shift,
        Ctrl,
        Alt,
        BS,
        ENTER,
        F5,
        Del,
        Count
    };

    enum inputMode {
        Editor,
        Game,
        Default
    };

    class inputs {
    public:
        inputs();

        void setInputMode(inputMode mode);
        void keyPressed  (const key &key);
        void keyReleased (const key &key);
        bool isKeyPressed(const key &key) const;
        inputMode getInputMode() const;
        double mousex, mousey;
        int mouseEnter{ 0 }, teclado_action{ 0 }, mods{ 0 }, mouseButton{ 0 };
        char letra{ '\0' };
        bool mouseClick{ false }, char_press{ false };
    protected:
        virtual void handleKey(const key &key);
    private:
        inputMode currentMode{ inputMode::Editor };
        std::unordered_map<key, bool> keyStates;
    };

    // Callback de teclado GLFW
    extern "C" void mousePosCallBack(GLFWwindow* window, double x, double y);
    extern "C" void callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    extern "C" void mouseButtonCallBack(GLFWwindow* window, int a, int b, int c);
    extern "C" void charCallback(GLFWwindow* window, unsigned int codepoint);
}