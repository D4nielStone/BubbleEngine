#ifndef DEBUG_H
#define DEBUG_H
#include "becommons.hpp"
#include "vector"
#include "utility"
#include "string"

namespace Debug
{
    enum BECOMMONS_DLL_API Tipo
    {
        Mensagem,
        Erro,
        Alerta
    };

    BECOMMONS_DLL_API void cout(const std::string &msg);
    BECOMMONS_DLL_API void emitir(Tipo t, std::string msg);
    BECOMMONS_DLL_API void emitir(std::string t, std::string msg);
    
    BECOMMONS_DLL_API std::vector<std::string> obterMensagems();
}
#endif