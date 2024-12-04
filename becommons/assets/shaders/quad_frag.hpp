inline const char* quad_frag = 
R"(
#version 330 core

in vec4 cor;

out vec4 FragColor;

void main()
{
    vec4 result = cor;

    FragColor = result;
}
)";