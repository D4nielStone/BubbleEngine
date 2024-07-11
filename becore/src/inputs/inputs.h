#ifndef INPUTS_H
#define INPUTS_H

#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include <functional>

enum class Key {
    W = 0, A = 1, S = 2, D = 3, E = 4, Q = 5, Shift = 6, Ctrl = 7, Alt = 8, Count = 9
};

namespace std {
    template <>
    struct hash<Key> {
        std::size_t operator()(const Key& k) const {
            return std::hash<int>()(static_cast<int>(k));
        }
    };
}


enum class InputMode {
    Editor,
    Game
};

class Inputs {
public:
    Inputs();

    void setInputMode(InputMode mode);
    void keyPressed(Key key);
    void keyReleased(Key key);
    bool isKeyPressed(Key key);

protected:
    virtual void handleKey(Key key);
    virtual void handleGameKey(Key key);
    virtual void handleEditorKey(Key key);

private:
    InputMode currentMode;
    std::unordered_map<Key, bool> keyStates;
};

// Callback de teclado GLFW
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif // INPUTS_H
