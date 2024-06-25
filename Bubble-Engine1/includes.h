#pragma once
#include <vector>
#include <iostream>
#include <cstdio>

struct Color {
    float r = 1, g = 1, b = 1;
};

struct Material {
    std::string nome;
    Color ambiente;
    Color difusa;
    Color especular;
};

struct Vertex {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> uvs;
    std::vector<float> normals;
};