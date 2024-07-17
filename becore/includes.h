#pragma once
#include <vector>
#include <iostream>
#include <cstdio>

struct Color {
    float r = 0.7, g = 0.7, b = 0.7;
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