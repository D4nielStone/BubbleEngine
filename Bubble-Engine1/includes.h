#pragma once
#include <vector>
#include <iostream>
#include <cstdio>

struct Color {
    float r = 1, g = 1, b = 1;
};

struct Material {
    std::string name;
    Color ambient;
    Color diffuse;
    Color specular;
};

struct Vertex {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> uvs;
    std::vector<float> normals;
};