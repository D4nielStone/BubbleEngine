#include "gameinputs.h"
#include <iostream>

void GameInputs::handleGameKey(Key key) {
    std::cout << "Game mode: Key pressed: " << static_cast<int>(key) << std::endl;
}

void GameInputs::handleEditorKey(Key key) {
    std::cout << "Editor mode: Key pressed: " << static_cast<int>(key) << std::endl;
}
