/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
/* Definição da estrutura Vetor2 */
template<class T>
struct Vetor2
{
    T x, y;

    // Construtor com parâmetros
    Vetor2(T x, T y) : x(x), y(y){}

    // Construtor padrão
    Vetor2() : x(T{}), y(T{}) {}

    // Operador de soma com outro Vetor2
    Vetor2 operator+(const Vetor2& other) const
    {
        return Vetor2{ x + other.x, y + other.y };
    }

    // Operador de soma com um escalar
    Vetor2 operator+(const T& scalar) const
    {
        return Vetor2{ x + scalar, y + scalar };
    }

    // Operador de soma acumulada com outro Vetor2
    Vetor2& operator+=(const Vetor2& other)
    {
        x += other.x;
        y += other.y;
    
        return *this;
    }

    // Operador de soma acumulada com um escalar
    Vetor2& operator+=(const T& scalar)
    {
        x += scalar;
        y += scalar;
        return *this;
    }

    // Operador de subtração com outro Vetor2
    Vetor2 operator-(const Vetor2& other) const
    {
        return Vetor2{ x - other.x, y - other.y };
    }

    // Operador de subtração com um escalar
    Vetor2 operator-(const T& scalar) const
    {
        return Vetor2{ x - scalar, y - scalar};
    }

    // Operador de subtração acumulada com outro Vetor2
    Vetor2& operator-=(const Vetor2& other)
    {
        x -= other.x;
        y -= other.y;
    
        return *this;
    }

    // Operador de subtração acumulada com um escalar
    Vetor2& operator-=(const T& scalar)
    {
        x -= scalar;
        y -= scalar;
        return *this;
    }

    // Operador de multiplicação com outro Vetor2
    Vetor2 operator*(const Vetor2& other) const
    {
        return Vetor2{ x * other.x, y * other.y };
    }

    // Operador de multiplicação com um escalar
    Vetor2 operator*(const T& scalar) const
    {
        return Vetor2{ x * scalar, y * scalar };
    }

    // Operador de multiplicação acumulada com outro Vetor2
    Vetor2& operator*=(const Vetor2& other)
    {
        x *= other.x;
        y *= other.y;
    
        return *this;
    }

    // Operador de multiplicação acumulada com um escalar
    Vetor2& operator*=(const T& scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Operador de igualdade
    bool operator==(const Vetor2& other) const
    {
        return x == other.x && y == other.y;
    }

    // Operador de diferença
    bool operator!=(const Vetor2& other) const
    {
        return !(*this == other);
    }

};