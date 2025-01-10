/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
/* Definição da estrutura vetor3 */

namespace bubble
{
    template<class T>
    struct vetor3
    {
        T x, y, z;

        // Construtor com parâmetros
        vetor3(T x, T y, T z) : x(x), y(y), z(z) {}

        // Construtor padrão
        vetor3() : x(T{}), y(T{}), z(T{}) {}

        // Operador de soma com outro vetor3
        vetor3 operator+(const vetor3& other) const
        {
            return vetor3{ x + other.x, y + other.y, z + other.z };
        }

        // Operador de soma com um escalar
        vetor3 operator+(const T& scalar) const
        {
            return vetor3{ x + scalar, y + scalar, z + scalar };
        }

        // Operador de soma acumulada com outro vetor3
        vetor3& operator+=(const vetor3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        // Operador de soma acumulada com um escalar
        vetor3& operator+=(const T& scalar)
        {
            x += scalar;
            y += scalar;
            z += scalar;
            return *this;
        }

        // Operador de subtração com outro vetor3
        vetor3 operator-(const vetor3& other) const
        {
            return vetor3{ x - other.x, y - other.y, z - other.z };
        }

        // Operador de subtração com um escalar
        vetor3 operator-(const T& scalar) const
        {
            return vetor3{ x - scalar, y - scalar, z - scalar };
        }

        // Operador de subtração acumulada com outro vetor3
        vetor3& operator-=(const vetor3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        // Operador de subtração acumulada com um escalar
        vetor3& operator-=(const T& scalar)
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;
            return *this;
        }

        // Operador de multiplicação com outro vetor3
        vetor3 operator*(const vetor3& other) const
        {
            return vetor3{ x * other.x, y * other.y, z * other.z };
        }

        // Operador de multiplicação com um escalar
        vetor3 operator*(const T& scalar) const
        {
            return vetor3{ x * scalar, y * scalar, z * scalar };
        }

        // Operador de multiplicação acumulada com outro vetor3
        vetor3& operator*=(const vetor3& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        // Operador de multiplicação acumulada com um escalar
        vetor3& operator*=(const T& scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        // Operador de igualdade
        bool operator==(const vetor3& other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }

        // Operador de diferença
        bool operator!=(const vetor3& other) const
        {
            return !(*this == other);
        }

    };
}

typedef bubble::vetor3<float> vet3;