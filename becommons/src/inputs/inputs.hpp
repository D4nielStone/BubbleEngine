#pragma once
#include <iostream>
#include <unordered_map>
#include <functional>
#include "src/util/includes.hpp"
#include "becommons.hpp"

struct GLFWwindow;
enum BECOMMONS_DLL_API Key {
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
    Count
};

namespace std {
    template <>
    struct hash<Key> {
        std::size_t operator()(const Key& k) const {
            return std::hash<int>()(static_cast<int>(k));
        }
    };
}


enum BECOMMONS_DLL_API InputMode {
    Editor,
    Game
};
namespace Bubble
{
    namespace Inputs
    {

        class BECOMMONS_DLL_API Inputs {
        public:
            Inputs();

            void setInputMode(InputMode mode);
            void keyPressed(Key key);
            void keyReleased(Key key);
            bool isKeyPressed(Key key) const;
            InputMode getInputMode() const;
            double mousex, mousey;
            int mouseEnter = 0, teclado_action = 0, mods = 0;
            char letra = '\0';
            bool mouseClick, char_press = false;
        protected:
            virtual void handleKey(Key key);
        private:
            InputMode currentMode = InputMode::Editor;
            std::unordered_map<Key, bool> keyStates;
        };
    }
}

// Callback de teclado GLFW
extern "C" BECOMMONS_DLL_API void mousePosCallBack(GLFWwindow* window, double x, double y);
extern "C" BECOMMONS_DLL_API void callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods);
extern "C" BECOMMONS_DLL_API void mouseButtonCallBack(GLFWwindow* window, int a, int b, int c);
extern "C" BECOMMONS_DLL_API void charCallback(GLFWwindow* window, unsigned int codepoint);