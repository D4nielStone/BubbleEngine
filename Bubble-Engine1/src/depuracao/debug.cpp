#include "debug.h"
#include "fmt/color.h"
#include "fmt/core.h"

void Debug::emitir(Debug::Tipo t, const char* msg)
{
    switch (t)
    {
    case Debug::Tipo::Mensagem:
        fmt::print(fg(fmt::color::blue), "[ Mensagem ]");
        break;
    case Debug::Tipo::Erro:
        fmt::print(fg(fmt::color::red), "[   ERRO   ]");
        break;
    case Debug::Tipo::Alerta:
        fmt::print(fg(fmt::color::yellow), "[ ALERTA! ]");
        break;
    }
    fmt::print(fg(fmt::color::white), "{}\n", msg);
}
