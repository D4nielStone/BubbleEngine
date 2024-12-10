
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#pragma once
#include <vector>
#include <iostream>
#include <cstdio>
#include <glad/glad.h>
#include <becommons.hpp>
#include <glm/vec3.hpp>
#include "src/arquivadores/shader.hpp"
#include <any>
#include <type_traits>
//namespace Bubble { namespace Componentes { class Camera; } }
// Definição da estrutura Vector4
template<class T>
struct Vector4
{
    T x, y, w, h;

    Vector4(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
    Vector4() = default;
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
    // Operador de diferença (!=)
    bool operator!=(const Vector4& other) const
    {
        return !(*this == other);
    }
};


template<class T>
struct Vector3
{
    T x, y, z;

    // Construtor com parâmetros
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    // Construtor padrão
    Vector3() : x(T{}), y(T{}), z(T{}) {}

    // Operador de soma com outro Vector3
    Vector3 operator+(const glm::vec3& other) const
    {
        return Vector3{ x + other.x, y + other.y, z + other.z };
    }
    
    // Operador de soma com outro Vector3
    Vector3 operator+(const Vector3& other) const
    {
        return Vector3{ x + other.x, y + other.y, z + other.z };
    }

    // Operador de soma com um escalar
    Vector3 operator+(const T& scalar) const
    {
        return Vector3{ x + scalar, y + scalar, z + scalar };
    }

    // Operador de soma acumulada com outro Vector3
    Vector3& operator+=(const Vector3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    // Operador de soma acumulada com um escalar
    Vector3& operator+=(const T& scalar)
    {
        x += scalar;
        y += scalar;
        z += scalar;
        return *this;
    }

    // Operador de subtração com outro Vector3
    Vector3 operator-(const Vector3& other) const
    {
        return Vector3{ x - other.x, y - other.y, z - other.z };
    }

    // Operador de subtração com um escalar
    Vector3 operator-(const T& scalar) const
    {
        return Vector3{ x - scalar, y - scalar, z - scalar };
    }

    // Operador de subtração acumulada com outro Vector3
    Vector3& operator-=(const Vector3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    // Operador de subtração acumulada com um escalar
    Vector3& operator-=(const T& scalar)
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        return *this;
    }

    // Operador de multiplicação com outro Vector3
    Vector3 operator*(const Vector3& other) const
    {
        return Vector3{ x * other.x, y * other.y, z * other.z };
    }

    // Operador de multiplicação com um escalar
    Vector3 operator*(const T& scalar) const
    {
        return Vector3{ x * scalar, y * scalar, z * scalar };
    }

    // Operador de multiplicação acumulada com outro Vector3
    Vector3& operator*=(const Vector3& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    // Operador de multiplicação acumulada com um escalar
    Vector3& operator*=(const T& scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    // Operador de atribuicao
    Vector3& operator=(const glm::vec3& eq)
    {
        x = eq.x;
        y = eq.y;
        z = eq.z;
        return *this;
    }

    // Operador de igualdade
    bool operator==(const Vector3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    // Operador de diferença
    bool operator!=(const Vector3& other) const
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

    // Operador de diferença (!=)
    bool operator!=(const Vector2& other) const
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

    // Operador de diferença (!=)
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
    // Operador de diferença (!=)
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
    // Operador de diferença (!=)
    bool operator!=(const Color& other) const
    {
        return !(*this == other);
    }
};

struct Textura {
    unsigned int ID{ 0 };
    std::string tipo{ "" };
    std::string path{ "" };

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
    Bubble::Arquivadores::Shader* shader{ nullptr };
    float shininess = 32.f, reflexao = 0;
    size_t ID{};
    std::vector<Textura> texturas;
    std::string nome{ "material sem nome" };
    ~Material() { if (shader) delete shader; };
    void bind();
};

struct Malha {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> uvs;
    std::vector<float> normals;
    Material material;
    std::string nome = "malha sem nome";
    unsigned int VAO = 0, EBO = 0, VBO = 0;
    bool carregado = false;
    size_t id{};
};

struct Node
{
    std::vector<Node> filhos;
    std::vector<Malha> malhas;
    /*std::shared_ptr<Bubble::Componentes::Camera> camera;*/
    std::vector<Luz> luzes;
    std::string nome = "node sem nome";
    int metrica{ 1 }; ///< medida de escala, 1 metro = 100 centimetros
    glm::mat4 transformacao;
};

struct LetraRect
{
    Vector4<float> rect;
    unsigned int ID;
};


using CheckBoxID = std::pair<bool*, const char*>;
using CaixaDeTextoID = std::pair<std::string*, const char*>;
using Vector3ID = std::pair<Vector3<float>*, const char*>;
using SeletorDeCorID = std::pair<Color*, const char*>;
using ArvoreID = std::pair<std::vector<std::any>, const char*>;

inline Malha rect_vertex;
inline Malha linha_vertex;
inline Malha halfcircle_vertex;

// cores
const Color ROXO_ESCURO = Color(0.17f, 0.14f, 0.2f, 1.f);
const Color ROXO_ESCURO_2 = Color(0.17f, 0.14f, 0.2f, 0.5f);
const Color ROXO_CLARO = Color( 0.25f, 0.21f, 0.29f, 1.f );
const Color ROXO_CLARO_2 = Color(0.30F, 0.23F, 0.40F);