/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
/* Defini��o da estrutura Vetor2 */
template<class T>
struct Vetor2
{
    T x, y;

    // Construtor com par�metros
    Vetor2(T x, T y) : x(x), y(y){}

    // Construtor padr�o
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

    // Operador de subtra��o com outro Vetor2
    Vetor2 operator-(const Vetor2& other) const
    {
        return Vetor2{ x - other.x, y - other.y };
    }

    // Operador de subtra��o com um escalar
    Vetor2 operator-(const T& scalar) const
    {
        return Vetor2{ x - scalar, y - scalar};
    }

    // Operador de subtra��o acumulada com outro Vetor2
    Vetor2& operator-=(const Vetor2& other)
    {
        x -= other.x;
        y -= other.y;
    
        return *this;
    }

    // Operador de subtra��o acumulada com um escalar
    Vetor2& operator-=(const T& scalar)
    {
        x -= scalar;
        y -= scalar;
        return *this;
    }

    // Operador de multiplica��o com outro Vetor2
    Vetor2 operator*(const Vetor2& other) const
    {
        return Vetor2{ x * other.x, y * other.y };
    }

    // Operador de multiplica��o com um escalar
    Vetor2 operator*(const T& scalar) const
    {
        return Vetor2{ x * scalar, y * scalar };
    }

    // Operador de multiplica��o acumulada com outro Vetor2
    Vetor2& operator*=(const Vetor2& other)
    {
        x *= other.x;
        y *= other.y;
    
        return *this;
    }

    // Operador de multiplica��o acumulada com um escalar
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

    // Operador de diferen�a
    bool operator!=(const Vetor2& other) const
    {
        return !(*this == other);
    }

};