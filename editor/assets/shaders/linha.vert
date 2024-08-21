#version 330 core

struct Linha
{
    vec2 pos1;
    vec2 pos2;
};

uniform Linha linha;

void main()
{
    if (gl_VertexID == 0)
        gl_Position = vec4(linha.pos1, 0.0, 1.0);
    else
        gl_Position = vec4(linha.pos2, 0.0, 1.0);
}