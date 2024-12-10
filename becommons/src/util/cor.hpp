/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
/* Definição da estrutura Cor */
struct Cor
{
    float r = 0.26f, g = 0.21f, b = 0.28f, a = 1.f;
    Cor(const float& r, const float& g, const float& b, const float& a) : r(r), g(g), b(b), a(a) {};
    Cor(const float& r, const float& g, const float& b) : r(r), g(g), b(b) {};
    Cor() = default;
    // Operador de soma (+)
    Cor operator+(const float other) const
    {
        return { r + other, g + other, b + other, 1.f };
    }
    // Operador de igualdade (==)
    bool operator==(const Cor& other) const
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
    // Operador de diferença (!=)
    bool operator!=(const Cor& other) const
    {
        return !(*this == other);
    }
};