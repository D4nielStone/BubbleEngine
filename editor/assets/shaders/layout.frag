#version 330 core

in vec2 Uv;

out vec4 FragColor;

uniform vec3 cor;

void main()
{
    vec3 result = cor;
    FragColor = vec4(result, 1.0);
}
