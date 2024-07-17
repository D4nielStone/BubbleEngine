#ifndef DEBUG_H
#define DEBUG_H
#include "becore.h"
#include "vector"
#include "utility"
#include "string"

namespace Debug
{
    enum BECORE_DLL_API Tipo
    {
        Mensagem,
        Erro,
        Alerta
    };

    BECORE_DLL_API void emitir(Tipo t, std::string msg);
    
    BECORE_DLL_API std::vector<std::pair<Debug::Tipo, const char*>>* obterMensagems();
}
#endif