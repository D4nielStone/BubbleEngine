#version 330 core

layout (location = 0) in vec3 aPos;  // Posição do vértice
layout (location = 1) in vec3 aNormal; // Normal do vértice

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
float outlineThickness = 0.1f; // Espessura do contorno

void main()
{
    // Calcula a nova posição do vértice, deslocada na direção da normal
    vec3 newPosition = aPos + aNormal * outlineThickness;

    // Calcula a posição final do vértice no espaço de projeção
    gl_Position = projection * view * model * vec4(newPosition, 1.0);
}
