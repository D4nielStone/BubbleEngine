#version 330 core

in vec2 Uv;
in vec3 cor;

out vec4 FragColor;

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
