#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
uniform vec3 viewPos;

out vec4 FragColor;

struct Textura
{
    bool ativado;
};

struct Material
{
    vec3 cor_difusa;
};

// Texturas
uniform sampler2D textura_albedo; // Albedo (cor difusa)
uniform Textura textura_albedo_;

uniform sampler2D textura_metallic; // Metalness
uniform Textura textura_metallic_;

uniform sampler2D textura_roughness; // Roughness
uniform Textura textura_roughness_;

uniform sampler2D textura_ambient_occlusion; // Ambient Occlusion
uniform Textura textura_ambient_occlusion_;

uniform sampler2D textura_emissiva; // Emissive
uniform Textura textura_emissiva_;

uniform sampler2D textura_normais; // Normal Map
uniform Textura textura_normais_;

uniform Material material;

vec3 calculateLighting(vec3 norm, vec3 viewDir, vec3 albedo, float metallic, float roughness, vec3 emissive)
{
    vec3 lightPos = vec3(5.f); // Posição da luz (exemplo)
    vec3 lightColor = vec3(1.f); // Cor da luz

    // Vectors
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 halfDir = normalize(lightDir + viewDir);

    // NDF (Normal Distribution Function) - GGX/Trowbridge-Reitz
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(norm, halfDir), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    float D = a2 / (3.14159265 * denom * denom);

    // Geometry Function - Smith's Schlick-GGX
    float k = a / 2.0;
    float NdotV = max(dot(norm, viewDir), 0.0);
    float NdotL = max(dot(norm, lightDir), 0.0);
    float G = (NdotL * (1.0 - k) + k) * (NdotV * (1.0 - k) + k);
    G = G / (4.0 * NdotL * NdotV + 0.001);

    // Fresnel - Schlick approximation
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - NdotH, 5.0);

    // kS is equal to Fresnel
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    // Final color
    vec3 irradiance = lightColor * NdotL;
    vec3 diffuse = kD * albedo / 3.14159265;
    vec3 ambient = texture(textura_ambient_occlusion, TexCoords).rgb;

    return ambient + (diffuse) * irradiance + emissive;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Retrieve textures
    vec3 albedo = textura_albedo_.ativado ? texture(textura_albedo, TexCoords).rgb : vec3(1.0);
    float metallic = textura_metallic_.ativado ? texture(textura_metallic, TexCoords).r : 0.0;
    float roughness = textura_roughness_.ativado ? texture(textura_roughness, TexCoords).r : 1.0;
    vec3 emissive = textura_emissiva_.ativado ? texture(textura_emissiva, TexCoords).rgb : vec3(0.0);

    // Lighting calculation
    vec3 lighting = calculateLighting(norm, viewDir, albedo, metallic, roughness, emissive);

    // Brightness multiplier
    vec3 ambient_color = vec3(0.9f);
    FragColor = vec4(lighting * material.cor_difusa * ambient_color, 1.0);
}
