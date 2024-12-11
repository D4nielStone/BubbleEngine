
inline const char* phong_vert = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 Normal;
out vec3 Position;
out vec2 Uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Uv = aUV;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = aPos;
    gl_Position = projection * view * vec4(aPos, 1.f);
} 
)";