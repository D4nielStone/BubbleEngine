const char* texto_frag = R"(
#version 330 core

in vec2 Uv;
in vec3 cor_texto;
out vec4 FragColor;

uniform sampler2D textura;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textura, Uv).r);
    FragColor = vec4(cor_texto, 1.0) * sampled;
}
)";