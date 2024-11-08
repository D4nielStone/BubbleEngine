#pragma once
#include <vector>
#include <iostream>
#include <cstdio>
#include <iostream>
#include <becommons.hpp>
#include "glm/glm.hpp"
#include "src/arquivadores/shader.hpp"
#include <any>

// Defini��o da estrutura Vector4
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
    // Operador de igualdade (==)
    bool operator==(const Vector4& other) const
    {
        return x == other.x && y == other.y && w == other.w && h == other.h;
    }
    // Operador de diferen�a (!=)
    bool operator!=(const Vector4& other) const
    {
        return !(*this == other);
    }
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
    // Operador de igualdade (==)
    bool operator==(const Vector4f& other) const
    {
        return x == other.x && y == other.y && w == other.w && z == other.z;
    }
    // Operador de diferen�a (!=)
    bool operator!=(const Vector4f& other) const
    {
        return !(*this == other);
    }
};

struct Vector2
{
    int x = 0, y = 0;
    // Operador de soma (+)
    Vector2 operator+(const Vector2& other) const
    {
        return { x + other.x && y + other.y };
    }
    // Operador de subtacao (-)
    Vector2 operator-(const Vector2& other) const
    {
        return { x - other.x && y - other.y };
    }
    // Operador de igualdade (==)
    bool operator==(const Vector2& other) const
    {
        return x == other.x && y == other.y;
    }

    // Operador de diferen�a (!=)
    bool operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }
};
struct Vector3
{
    int x = 0, y = 0, z = 0;
    // Operador de soma (+)
    Vector3 operator+(const Vector3& other) const
    {
        return { x + other.x && y + other.y && z + other.z };
    }
    // Operador de igualdade (==)
    bool operator==(const Vector3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    // Operador de diferen�a (!=)
    bool operator!=(const Vector3& other) const
    {
        return !(*this == other);
    }
};
struct Vector2f
{
    float x = 0, y = 0;
    // Operador de igualdade (==)
    bool operator==(const Vector2f& other) const
    {
        return x == other.x && y == other.y;
    }

    // Operador de diferen�a (!=)
    bool operator!=(const Vector2f& other) const
    {
        return !(*this == other);
    }
    // Operador de soma (+)
    Vector2f operator+(const Vector2f& other) const
    {
        return {x + other.x, y+other.y};
    }
};

struct Size
{
    int width = 1, height = 1;
    // Operador de igualdade (==)
    bool operator==(const Size& other) const
    {
        return width == other.width && height == other.height;
    }
    // Operador de diferen�a (!=)
    bool operator!=(const Size& other) const
    {
        return !(*this == other);
    }
};

struct Color 
{
    float r = 0.26f, g = 0.21f, b = 0.28f, a = 1.f;
    Color( const float &r, const float &g, const float &b, const float &a) : r(r), g(g), b(b), a(a){};
    Color( const float &r, const float &g, const float &b) : r(r), g(g), b(b){};
    Color() = default;
    // Operador de soma (+)
    Color operator+(const float other) const
    {
        return { r + other, g + other, b + other, 1.f };
    }
    // Operador de igualdade (==)
    bool operator==(const Color& other) const
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
    // Operador de diferen�a (!=)
    bool operator!=(const Color& other) const
    {
        return !(*this == other);
    }
};

struct Textura {
    unsigned int ID{ 0 };
    std::string tipo{ "" };
    const char* path{ "" };

    // Operador de igualdade
    bool operator==(const Textura& other) const {
        return ID == other.ID && tipo == other.tipo && std::string(path) == std::string(other.path);
    }
};

struct Luz
{
    glm::vec3 posicao;
    Color cor_ambiente;
    Color cor_difusa;
    Color cor_especular;
};

struct Material {
    Color difusa;
    Color especular;
    float shininess = 32.f, reflexao = 0;
    size_t ID{};
    std::vector<Textura> texturas;
    std::string nome{ "material sem nome" };
};

struct Vertex {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> uvs;
    std::vector<float> normals;
    Material material;
    std::string nome = "malha sem nome";
    unsigned int VAO = 0, EBO = 0, VBO = 0;
    bool carregado = false;
};

struct Node
{
    std::vector<Node> filhos;
    std::vector<Vertex> malhas;
    std::string nome = "node sem nome";
    glm::mat4 transformacao;
};

struct LetraRect
{
    Vector4f rect;
    unsigned int ID;
};


using CheckBoxID = std::pair<bool*, const char*>;
using CaixaDeTextoID = std::pair<std::string*, const char*>;
using SeletorDeCorID = std::pair<Color*, const char*>;
using ArvoreID = std::pair<std::vector<std::any>, const char*>;

extern BECOMMONS_DLL_API Vertex rect_vertex;
extern BECOMMONS_DLL_API Vertex linha_vertex;
extern BECOMMONS_DLL_API Vertex halfcircle_vertex;