#ifndef INCLUDES_H
#define INCLUDES_H
#include <vector>
#include <iostream>
#include <cstdio>
#include <iostream>

// Definição da estrutura Vector2
struct Vector2
{
    float x = 1, y = 1;
    int w = 0, h = 0;
    // Deve somar
    Vector2 operator+(const Vector2& other) const
    {
        return Vector2{ x + other.x, y + other.y, static_cast<int>(w + other.w), static_cast<int>(h + other.h) };
    };
    Vector2 operator+(const float other) const
    {
        return Vector2{ x + other, y + other, static_cast<int>(w + other), static_cast<int>(h + other) };
    };
    Vector2& operator+=(const Vector2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    };
    Vector2& operator+=(const float other)
    {
        x += other;
        y += other;
        return *this;
    };
    // Deve subtrair
    Vector2 operator-(const Vector2& other) const
    {
        return Vector2{ x - other.x, y - other.y, static_cast<int>(w - other.w), static_cast<int>(h - other.h) };
    };
    Vector2 operator-(const float other) const
    {
        return Vector2{ x - other, y - other, static_cast<int>(w - other), static_cast<int>(h - other) };
    };
    Vector2& operator-=(const Vector2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    };
    Vector2& operator-=(const float other)
    {
        x -= other;
        y -= other;
        return *this;
    };
    //multi
    Vector2 operator*=(const Vector2& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    };
    Vector2 operator*(float other)
    {
        return Vector2{x * other, y * other, static_cast<int>( w * other), static_cast<int>( h * other)};
    };
    Vector2 operator*=(float other)
    {
        x *= other;
        y *= other;
        return *this;
    };
};

struct Vector3
{
    float x = 1, y = 1, z = 1;
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
    unsigned int VAO, EBO, VBO;
    // tem que adicionar se já foi criado buffers (bool)
};
struct Projeto
{
    std::string nome;
    std::string path;
};
#endif // !INCLUDES_H