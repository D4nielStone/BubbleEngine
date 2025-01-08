
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "debug.hpp"
#include <iostream>

std::vector<std::string> msgs;

void debug::cout(const std::string& msg)
{
    msgs.push_back(msg + "");
}
std::vector<std::string> debug::obterMensagems()
{
    return msgs;
}

void debug::emitir(Tipo t, std::string msg) 
{
#ifdef _DEBUG
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
#endif
}
void debug::emitir(std::string t, std::string msg)
{
#ifdef _DEBUG
	std::cout << "[" << t << "] " << msg << "\n";

#endif
}