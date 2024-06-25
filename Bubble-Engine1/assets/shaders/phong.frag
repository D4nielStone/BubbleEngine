#version 330 core

in vec3 FragPos;         // Fragment position in world space
in vec3 Normal;          // Normal vector at the fragment
vec3 LightDir = vec3(0.1,0.1,0);        // Direction to the light source

out vec4 FragColor;

vec3 lightColor = vec3(1, 1, 1);
vec3 materialColor = vec3(1, 1, 1);
vec3 ambientColor = vec3(1, 1, 1);
uniform float shininess;

void main()
{
    // Ambient lighting
    vec3 ambient = ambientColor * materialColor * 0.2;

    // Diffuse lighting
    float diff = max(dot(normalize(Normal), normalize(LightDir)), 0.0);
    vec3 diffuse = diff * lightColor * materialColor;

    // Specular lighting
    vec3 viewDir = normalize(-FragPos); // Direction from fragment to the camera
    vec3 reflectDir = reflect(-LightDir, Normal); // Reflected light direction
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightColor;

    // Final color
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
