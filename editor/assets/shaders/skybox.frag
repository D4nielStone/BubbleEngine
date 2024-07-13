#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube texturea;

void main()
{
    FragColor = texture(texturea, TexCoords);
}
