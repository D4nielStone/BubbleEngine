
// Copyright (c) 2024 Daniel Oliveira

#include "debug.hpp"


std::vector<std::string> msgs;

void Debug::cout(const std::string& msg)
{
    msgs.push_back(msg + "");
}
std::vector<std::string> Debug::obterMensagems()
{
    return msgs;
}

void Debug::emitir(Tipo t, std::string msg) {}
void Debug::emitir(std::string t, std::string msg) {}