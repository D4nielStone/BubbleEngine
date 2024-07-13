#ifndef GAMEINPUTS_H
#define GAMEINPUTS_H

#include "inputs.h"
#include "becore.h"

namespace Bubble
{
    namespace Inputs
    {
        class BECORE_DLL_API GameInputs : public Inputs {
        protected:
            void handleGameKey(Key key) override;
            void handleEditorKey(Key key) override;
        };
    }
}
#endif // GAMEINPUTS_H
