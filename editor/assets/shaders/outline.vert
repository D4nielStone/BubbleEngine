#version 330 core

layout (location = 0) in vec3 aPos;  // Posi��o do v�rtice
layout (location = 1) in vec3 aNormal; // Normal do v�rtice

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
float outlineThickness = 0.1f; // Espessura do contorno

void main()
{
    // Calcula a nova posi��o do v�rtice, deslocada na dire��o da normal
    vec3 newPosition = aPos + aNormal * outlineThickness;

    // Calcula a posi��o final do v�rtice no espa�o de proje��o
    gl_Position = projection * view * model * vec4(newPosition, 1.0);
}
