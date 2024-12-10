/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
/* Definição da estrutura Vetor3 */
template<class T>
struct Vetor3
{
    T x, y, z;

    // Construtor com parâmetros
    Vetor3(T x, T y, T z) : x(x), y(y), z(z) {}

    // Construtor padrão
    Vetor3() : x(T{}), y(T{}), z(T{}) {}

    // Operador de soma com outro Vetor3
    Vetor3 operator+(const Vetor3& other) const
    {
        return Vetor3{ x + other.x, y + other.y, z + other.z };
    }

    // Operador de soma com um escalar
    Vetor3 operator+(const T& scalar) const
    {
        return Vetor3{ x + scalar, y + scalar, z + scalar };
    }

    // Operador de soma acumulada com outro Vetor3
    Vetor3& operator+=(const Vetor3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    // Operador de soma acumulada com um escalar
    Vetor3& operator+=(const T& scalar)
    {
        x += scalar;
        y += scalar;
        z += scalar;
        return *this;
    }

    // Operador de subtração com outro Vetor3
    Vetor3 operator-(const Vetor3& other) const
    {
        return Vetor3{ x - other.x, y - other.y, z - other.z };
    }

    // Operador de subtração com um escalar
    Vetor3 operator-(const T& scalar) const
    {
        return Vetor3{ x - scalar, y - scalar, z - scalar };
    }

    // Operador de subtração acumulada com outro Vetor3
    Vetor3& operator-=(const Vetor3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    // Operador de subtração acumulada com um escalar
    Vetor3& operator-=(const T& scalar)
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        return *this;
    }

    // Operador de multiplicação com outro Vetor3
    Vetor3 operator*(const Vetor3& other) const
    {
        return Vetor3{ x * other.x, y * other.y, z * other.z };
    }

    // Operador de multiplicação com um escalar
    Vetor3 operator*(const T& scalar) const
    {
        return Vetor3{ x * scalar, y * scalar, z * scalar };
    }

    // Operador de multiplicação acumulada com outro Vetor3
    Vetor3& operator*=(const Vetor3& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    // Operador de multiplicação acumulada com um escalar
    Vetor3& operator*=(const T& scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // Operador de igualdade
    bool operator==(const Vetor3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    // Operador de diferença
    bool operator!=(const Vetor3& other) const
    {
        return !(*this == other);
    }

};