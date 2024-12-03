/** @copyright Copyright (c) 2024 Daniel Oliveira */

const char* skybox_frag =
R"(
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}
)";