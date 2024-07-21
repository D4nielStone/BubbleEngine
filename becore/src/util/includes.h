#ifndef INCLUDES_H
#define INCLUDES_H
#include <vector>
#include <iostream>
#include <cstdio>

enum class Janela
{
	Editor,
	Projetos,
	Preview,
	Entidades,
	Arquivos,
	Inpetor,
	Console,
	MENU_Arquivos,
	MENU_CriarProjeto,
	MENU_Cena,
	MENU_Editar
};
#include <iostream>

// Definição da estrutura Vector2
struct Vector2
{
    float x = 0, y = 0;

    Vector2 operator+(const Vector2& other) const
    {
        return Vector2{ x + other.x, y + other.y };
    };
    Vector2 operator-(const Vector2& other) const
    {
        return Vector2{ x - other.x, y - other.y };
    };

    Vector2& operator+=(const Vector2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    };
    Vector2& operator-=(const Vector2& other)
    {
        x -= other.x;
        y -= other.y;
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
struct Material {
    Color difusa;
};

struct Vertex {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> uvs;
    std::vector<float> normals;
    unsigned int VAO, EBO, VBO;
};
struct Projeto
{
    std::string nome;
    std::string path;
};
#endif // !INCLUDES_H