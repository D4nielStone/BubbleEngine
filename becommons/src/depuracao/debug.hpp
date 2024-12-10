/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include <vector>
#include <utility>
#include <string>

enum Tipo
{
    Mensagem,
    Erro,
    Alerta
};
namespace Debug
{

    void cout(const std::string &msg);
    void emitir(Tipo t, std::string msg);
    void emitir(std::string t, std::string msg);
    
    std::vector<std::string> obterMensagems();
}