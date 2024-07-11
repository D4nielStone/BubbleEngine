// debug.h

#pragma once
#include "becore.h"

namespace Debug
{
    enum class BECORE_DLL_API Tipo
    {
        Mensagem,
        Erro,
        Alerta
    };

    BECORE_DLL_API void emitir(Tipo t, const char* msg);
}
