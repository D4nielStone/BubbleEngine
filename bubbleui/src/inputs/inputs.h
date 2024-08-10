#ifndef INPUTS_H
#define INPUTS_H
#include <iostream>
#include <unordered_map>
#include <functional>
#include "src/util/includes.h"
#include "bubbleui.h"
class GLFWwindow;
enum class BEUI_DLL_API Key {
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


enum BEUI_DLL_API InputMode {
    Editor,
    Game
};
namespace Bubble
{
    namespace Inputs
    {

        class BEUI_DLL_API Inputs {
        public:
            Inputs();

            void setInputMode(InputMode mode);
            void keyPressed(Key key);
            void keyReleased(Key key);
            bool isKeyPressed(Key key) const;
            InputMode getInputMode() const;
            double mousex, mousey;
            int mouseEnter;
            bool mouseClick;
        protected:
            virtual void handleKey(Key key);
        private:
            InputMode currentMode = InputMode::Editor;
            std::unordered_map<Key, bool> keyStates;
        };
    }
}

// Callback de teclado GLFW
extern "C" BEUI_DLL_API void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
extern "C" BEUI_DLL_API void mousePosCallBack(GLFWwindow* window, double x, double y);
extern "C" BEUI_DLL_API void mouseButtonCallBack(GLFWwindow* window, int a, int b, int c);

#endif // INPUTS_H
