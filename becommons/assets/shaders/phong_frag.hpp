inline const char* phong_frag = R"(
#version 330 core
out vec4 FragColor;

in vec2 Uv; // Coordenadas da textura
in vec3 Normal;    // Normal da superfície
in vec3 Position;   // Posição do fragmento

struct Material
{
    vec4 cor_difusa;
    vec4 cor_especular;
    float brilho; // A intensidade do brilho especular (ex: shininess)
};

uniform Material material;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform bool texture_diffuse1_bool;
uniform bool uvMundo;

uniform bool recebe_luz;

void main()
{
    vec3 result;
    vec2 novoUv = uvMundo ? Position.xz : Uv;
    // Verifica se a textura existe ou se deve usar a cor base
    vec4 texColor = texture(texture_diffuse1, novoUv);  // Obtém a cor da textura
    vec4 baseColor = (texture_diffuse1_bool) ? texColor : material.cor_difusa;  // Cor final após aplicar a textura ou a cor base
    if(recebe_luz)
    {

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(viewPos);

    // Calculando a iluminação difusa
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(baseColor);


    // Combina a iluminação difusa, especular e a cor ambiente
    result = diffuse;
    }
    else
    {    
        result = baseColor.xyz;
    }
    // Aplica o resultado final ao fragmento
    FragColor = vec4(result, baseColor.w);
}
)";
