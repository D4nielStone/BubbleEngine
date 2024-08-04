#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform sampler2D textura_difusa;

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1.0); // lightColor é 1.0, ajustado aqui

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(viewPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0); // lightColor é 1.0, ajustado aqui

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // ajuste do brilho
    vec3 specular = specularStrength * spec * vec3(1.0); // lightColor é 1.0, ajustado aqui

    // Combine results
    vec3 result = ambient + diffuse + specular;

    // Apply texture
    vec4 texColor = texture(textura_difusa, TexCoords);
    FragColor = vec4(result, 1.0);
}
