/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include <string>
struct Textura {
    unsigned int id;
    std::string tipo, path;

    // Operador de igualdade
    bool operator==(const Textura& other) const {
        return id == other.id && tipo == other.tipo && path == other.path;
    }
};