/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
/* Definição da estrutura Vetor4 */
template<class T>
struct Vetor4
{
    T x, y, w, h;

    Vetor4(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
    Vetor4() = default;
    // Deve somar
    Vetor4 operator+(const Vetor4& other) const
    {
        return Vetor4{ x + other.x, y + other.y, static_cast<int>(w + other.w), static_cast<int>(h + other.h) };
    };
    Vetor4 operator+(const float other) const
    {
        return Vetor4{ x + other, y + other, static_cast<int>(w + other), static_cast<int>(h + other) };
    };
    Vetor4& operator+=(const Vetor4& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    };
    Vetor4& operator+=(const float other)
    {
        x += other;
        y += other;
        return *this;
    };
    // Deve subtrair
    Vetor4 operator-(const Vetor4& other) const
    {
        return Vetor4{ x - other.x, y - other.y, static_cast<int>(w - other.w), static_cast<int>(h - other.h) };
    };
    Vetor4 operator-(const float other) const
    {
        return Vetor4{ x - other, y - other, static_cast<int>(w - other), static_cast<int>(h - other) };
    };
    Vetor4& operator-=(const Vetor4& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    };
    Vetor4& operator-=(const float other)
    {
        x -= other;
        y -= other;
        return *this;
    };
    // Deve multiplicar
    Vetor4 operator*(const Vetor4& other)
    {
        return Vetor4{ x * other.x, y * other.y, static_cast<int>(w * other.w), static_cast<int>(h * other.h) };
    };
    Vetor4 operator*(float other)
    {
        return Vetor4{ x * other, y * other, static_cast<int>(w * other), static_cast<int>(h * other) };
    };
    Vetor4 operator*=(const Vetor4& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    };
    Vetor4 operator*=(float other)
    {
        x *= other;
        y *= other;
        return *this;
    };
    // Deve dividir
    Vetor4 operator/(const Vetor4& other)
    {
        return Vetor4{ x / other.x, y / other.y, static_cast<int>(w / other.w), static_cast<int>(h / other.h) };
    };
    Vetor4 operator/(float other)
    {
        return Vetor4{ x / other, y / other, static_cast<int>(w / other), static_cast<int>(h / other) };
    };
    Vetor4 operator/=(const Vetor4& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    };
    Vetor4 operator/=(float other)
    {
        x /= other;
        y /= other;
        return *this;
    };
    // Operador de igualdade (==)
    bool operator==(const Vetor4& other) const
    {
        return x == other.x && y == other.y && w == other.w && h == other.h;
    }
    // Operador de diferença (!=)
    bool operator!=(const Vetor4& other) const
    {
        return !(*this == other);
    }
};