
inline const char* phong_frag = R"(
#version 330 core
out vec4 FragColor;

struct Material
{
    vec3 cor_difusa;
    vec3 cor_especular;
    float shininess;
    float reflexao;
};
uniform Material material;

struct Luz
{
    vec3 posicao;
    vec3 cor_ambiente;
    vec3 cor_difusa;
    vec3 cor_especular;
};

uniform Luz luz;

in vec3 Normal;
in vec3 Position;
in vec2 UV;

uniform vec3 viewPos;
uniform samplerCube skybox;
uniform sampler2D textura_difusa; uniform bool textura_difusa_ativo;

void main()
{             
    // Vetor de visão
    vec3 I = normalize(Position - viewPos);
    
    // Vetor de reflexão
    vec3 R = reflect(I, normalize(Normal));
    
    // Iluminação ambiente
    vec3 ambient = material.cor_difusa;
    
    // Iluminação difusa
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(vec3(20, 1, 1) - Position);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = (diff * material.cor_difusa);
    
    // Iluminação especular
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (spec * material.cor_especular);
    
    // Cor da reflexão do skybox
    vec3 reflection = texture(skybox, -R).rgb * material.reflexao;
    
    // Combina iluminação Phong com a reflexão
    vec3 result = ambient + diffuse + specular + reflection;

    if(textura_difusa_ativo)
    {
    FragColor = vec4(result, 1.0) * texture(textura_difusa, UV);
    return;
    }


    FragColor = vec4(result, 1.0);
}
)";