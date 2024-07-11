#ifndef GAMEINPUTS_H
#define GAMEINPUTS_H

#include "inputs.h"

class GameInputs : public Inputs {
protected:
    void handleGameKey(Key key) override;
    void handleEditorKey(Key key) override;
};

#endif // GAMEINPUTS_H
