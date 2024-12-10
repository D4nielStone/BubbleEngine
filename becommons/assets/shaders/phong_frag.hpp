
inline const char* phong_frag = R"(
#version 330 core
out vec4 FragColor;

in vec2 Uv; // Coordenadas da textura
in vec3 Normal;    // Normal da superfície
in vec3 Position;   // Posição do fragmento

uniform sampler2D material_texture_diffuse1;
uniform sampler2D material_texture_diffuse2;
uniform sampler2D material_texture_diffuse3;

void main()
{             
     

    FragColor = texture(material_texture_diffuse1, Uv);
}
)";