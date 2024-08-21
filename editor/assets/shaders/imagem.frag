#version 330 core

in vec2 Uv;
out vec4 FragColor;
uniform sampler2D textura;
void main()
{
    FragColor = texture(textura, vec2(Uv.x, -Uv.y)).rgba;
}
