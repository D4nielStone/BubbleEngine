#include "gameinputs.h"
#include <iostream>
#include "string"
#include "src/depuracao/debug.h"

namespace Bubble::Inputs
{
    void GameInputs::handleGameKey(Key key) {
        //Debug::emitir(Debug::Tipo::Alerta, std::string(std::string("Game mode: Key pressed: ") + std::to_string(static_cast<int>(key))).c_str());
    }

    void GameInputs::handleEditorKey(Key key) {
        //Debug::emitir(Debug::Tipo::Alerta, std::string(std::string("Editor mode: Key pressed: ") + std::to_string(static_cast<int>(key))).c_str());
    }
}