#ifndef INCLUDES_H
#define INCLUDES_H
#include <vector>
#include <iostream>
#include <cstdio>

struct Color {
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
};
struct Projeto
{
    std::string nome;
    std::string path;
};
#endif // !INCLUDES_H