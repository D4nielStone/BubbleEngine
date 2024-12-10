/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
/* Defini��o da estrutura Vetor3 */
template<class T>
struct Vetor3
{
    T x, y, z;

    // Construtor com par�metros
    Vetor3(T x, T y, T z) : x(x), y(y), z(z) {}

    // Construtor padr�o
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

    // Operador de subtra��o com outro Vetor3
    Vetor3 operator-(const Vetor3& other) const
    {
        return Vetor3{ x - other.x, y - other.y, z - other.z };
    }

    // Operador de subtra��o com um escalar
    Vetor3 operator-(const T& scalar) const
    {
        return Vetor3{ x - scalar, y - scalar, z - scalar };
    }

    // Operador de subtra��o acumulada com outro Vetor3
    Vetor3& operator-=(const Vetor3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    // Operador de subtra��o acumulada com um escalar
    Vetor3& operator-=(const T& scalar)
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        return *this;
    }

    // Operador de multiplica��o com outro Vetor3
    Vetor3 operator*(const Vetor3& other) const
    {
        return Vetor3{ x * other.x, y * other.y, z * other.z };
    }

    // Operador de multiplica��o com um escalar
    Vetor3 operator*(const T& scalar) const
    {
        return Vetor3{ x * scalar, y * scalar, z * scalar };
    }

    // Operador de multiplica��o acumulada com outro Vetor3
    Vetor3& operator*=(const Vetor3& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    // Operador de multiplica��o acumulada com um escalar
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

    // Operador de diferen�a
    bool operator!=(const Vetor3& other) const
    {
        return !(*this == other);
    }

};