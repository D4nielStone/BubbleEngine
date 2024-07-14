#ifndef INPUTS_H
#define INPUTS_H

#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include <functional>
#include "becore.h"

enum class BECORE_DLL_API Key {
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


enum class BECORE_DLL_API InputMode {
    Editor,
    Game
};

class BECORE_DLL_API Inputs {
public:
    Inputs();

    void setInputMode(InputMode mode);
    void keyPressed(Key key);
    void keyReleased(Key key);
    bool isKeyPressed(Key key);
    InputMode getInputMode() const;

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
