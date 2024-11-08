#include "debug.hpp"
#include "fmt/color.h"
#include "fmt/core.h"


std::vector<std::string> msgs;

void Debug::cout(const std::string& msg)
{
    msgs.push_back(msg + "\n");
}

void Debug::emitir(Debug::Tipo t, std::string msg)
{
#ifdef _DEBUG
    //switch (t)
    //{
    //case Debug::Mensagem:
    //    fmt::print(fg(fmt::color::blue), "[ Mensagem ]");
    //    break;
    //case Debug::Erro:
    //    fmt::print(fg(fmt::color::red), "[   ERRO   ]");
    //    break;
    //case Debug::Alerta:
    //    fmt::print(fg(fmt::color::yellow), "[ ALERTA! ]");
    //    break;
    //}
    //fmt::print(fg(fmt::color::white), "{}\n", msg);
#endif // _DEBUG
}

void Debug::emitir(std::string t, std::string msg)
{
#ifdef _DEBUG
    //fmt::print(fg(fmt::color::dark_gray), "[{}]", t);
    //fmt::print(fg(fmt::color::white), "{}\n", msg);
#endif // _DEBUG
}

std::vector<std::string> Debug::obterMensagems()
{
    return msgs;
}

