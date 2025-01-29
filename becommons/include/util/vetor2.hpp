/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
/* Definição da estrutura vetor2 */

namespace bubble
{
    template<class T>
    struct vetor2
    {
        T x, y;

        // Construtor com parâmetros
        vetor2(T x, T y) : x(x), y(y) {}

        // Construtor padrão
        vetor2() : x(T{}), y(T{}) {}

        // Operador de soma com outro vetor2
        vetor2 operator+(const vetor2& other) const
        {
            return vetor2{ x + other.x, y + other.y };
        }

        // Operador de soma com um escalar
        vetor2 operator+(const T& scalar) const
        {
            return vetor2{ x + scalar, y + scalar };
        }

        // Operador de soma acumulada com outro vetor2
        vetor2& operator+=(const vetor2& other)
        {
            x += other.x;
            y += other.y;

            return *this;
        }

        // Operador de soma acumulada com um escalar
        vetor2& operator+=(const T& scalar)
        {
            x += scalar;
            y += scalar;
            return *this;
        }

        // Operador de subtração com outro vetor2
        vetor2 operator-(const vetor2& other) const
        {
            return vetor2{ x - other.x, y - other.y };
        }

        // Operador de subtração com um escalar
        vetor2 operator-(const T& scalar) const
        {
            return vetor2{ x - scalar, y - scalar };
        }

        // Operador de subtração acumulada com outro vetor2
        vetor2& operator-=(const vetor2& other)
        {
            x -= other.x;
            y -= other.y;

            return *this;
        }

        // Operador de subtração acumulada com um escalar
        vetor2& operator-=(const T& scalar)
        {
            x -= scalar;
            y -= scalar;
            return *this;
        }

        // Operador de multiplicação com outro vetor2
        vetor2 operator*(const vetor2& other) const
        {
            return vetor2{ x * other.x, y * other.y };
        }

        // Operador de multiplicação com um escalar
        vetor2 operator*(const T& scalar) const
        {
            return vetor2{ x * scalar, y * scalar };
        }

        // Operador de multiplicação acumulada com outro vetor2
        vetor2& operator*=(const vetor2& other)
        {
            x *= other.x;
            y *= other.y;

            return *this;
        }

        // Operador de multiplicação acumulada com um escalar
        vetor2& operator*=(const T& scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        // Operador de igualdade
        bool operator==(const vetor2& other) const
        {
            return x == other.x && y == other.y;
        }

        // Operador de diferença
        bool operator!=(const vetor2& other) const
        {
            return !(*this == other);
        }

    };
}

typedef bubble::vetor2<double> vet2;