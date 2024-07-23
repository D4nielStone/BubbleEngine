#version 330 core

in vec2 Uv;

out vec4 FragColor;

uniform vec3 cor;
uniform sampler2D textura;
uniform bool imagem;

void main()
{
    vec4 result = vec4(cor, 1.f);
    if (imagem)
    {
        result *= texture(textura, Uv).rgba;
    }
    FragColor = result;
}
