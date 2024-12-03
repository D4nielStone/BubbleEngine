
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "debug.hpp"
#include <iostream>

std::vector<std::string> msgs;

void Debug::cout(const std::string& msg)
{
    msgs.push_back(msg + "");
}
std::vector<std::string> Debug::obterMensagems()
{
    return msgs;
}

void Debug::emitir(Tipo t, std::string msg) 
#ifdef _DEBUG
{
	switch (t)
	{
	case Mensagem:
		std::cout << "[Mensagem] " << msg << "\n";
		break;
	case Erro:
		std::cout << "[Erro] " << msg << "\n";
		break;
	case Alerta:
		std::cout << "[Alerta] " << msg << "\n";
		break;
	default:
		break;
	}
}
#endif
void Debug::emitir(std::string t, std::string msg)
#ifdef _DEBUG
{
	std::cout << "[" << t << "] " << msg << "\n";

}
#endif