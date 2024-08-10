#ifndef INCLUDES_H
#define INCLUDES_H
#include <vector>
#include <iostream>
#include <cstdio>
#include <iostream>

// Definição da estrutura Vector4
struct Vector4
{
    float x = 1, y = 1;
    int w = 0, h = 0;

    // Deve somar
    Vector4 operator+(const Vector4& other) const
    {
        return Vector4{ x + other.x, y + other.y, static_cast<int>(w + other.w), static_cast<int>(h + other.h) };
    };
    Vector4 operator+(const float other) const
    {
        return Vector4{ x + other, y + other, static_cast<int>(w + other), static_cast<int>(h + other) };
    };
    Vector4& operator+=(const Vector4& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    };
    Vector4& operator+=(const float other)
    {
        x += other;
        y += other;
        return *this;
    };
    // Deve subtrair
    Vector4 operator-(const Vector4& other) const
    {
        return Vector4{ x - other.x, y - other.y, static_cast<int>(w - other.w), static_cast<int>(h - other.h) };
    };
    Vector4 operator-(const float other) const
    {
        return Vector4{ x - other, y - other, static_cast<int>(w - other), static_cast<int>(h - other) };
    };
    Vector4& operator-=(const Vector4& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    };
    Vector4& operator-=(const float other)
    {
        x -= other;
        y -= other;
        return *this;
    };
    // Deve multiplicar
    Vector4 operator*(const Vector4& other)
    {
        return Vector4{x * other.x, y * other.y, static_cast<int>( w * other.w), static_cast<int>( h * other.h)};
    };
    Vector4 operator*(float other)
    {
        return Vector4{x * other, y * other, static_cast<int>( w * other), static_cast<int>( h * other)};
    };
    Vector4 operator*=(const Vector4& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    };
    Vector4 operator*=(float other)
    {
        x *= other;
        y *= other;
        return *this;
    };
    // Deve dividir
    Vector4 operator/(const Vector4& other)
    {
        return Vector4{x / other.x, y / other.y, static_cast<int>( w / other.w), static_cast<int>( h / other.h)};
    };
    Vector4 operator/(float other)
    {
        return Vector4{x / other, y / other, static_cast<int>( w / other), static_cast<int>( h / other)};
    };
    Vector4 operator/=(const Vector4& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    };
    Vector4 operator/=(float other)
    {
        x /= other;
        y /= other;
        return *this;
    };
};
struct Vector4f
{
    float x = 1, y = 1, z = 1, w = 1;

    // Deve somar
    Vector4f operator+(const Vector4f& other) const
    {
        return Vector4f{ x + other.x, y + other.y, z + other.z, w + other.w };
    };
    Vector4f operator+(const float other) const
    {
        return Vector4f{ x + other, y + other, z + other, w + other };
    };
};


struct Size
{
    int width = 1, height = 1;
};

struct Color 
{
    float r = 0.7f, g = 0.7f, b = 0.7f;
};
struct Textura
{
    unsigned int ID;
    const char* path = "R.jfif";
    bool gerado = false;
};

struct Material {
    Color difusa;
    Textura textura_difusa;
};

struct Vertex {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> uvs;
    std::vector<float> normals;
    unsigned int VAO = 0, EBO = 0, VBO = 0;
    bool carregado = false;
};
struct Projeto
{
    std::string nome;
    std::string path;
};

#endif // !INCLUDES_H