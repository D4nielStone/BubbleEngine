#version 330 core

in vec2 Uv;
in vec3 cor;

out vec4 FragColor;

uniform sampler2D textura;
uniform bool imagem, texto;
uniform vec3 cor_texto;

void main()
{
    vec4 result = vec4(cor, 1.f);
    if (imagem)
    {
        result *= texture(textura, Uv).rgba;
    }
    else if(texto)
    {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textura, Uv).r);
        result = vec4(cor_texto, 1.0) * sampled;
    }
    FragColor = result;
}
