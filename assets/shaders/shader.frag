#version 450

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec3 inFragPos;
layout(location = 2) in vec3 inViewPos;

layout (location = 0) out vec4 outColor;

vec3 lightPos = vec3(4, -3, 0);
vec3 lightColor = vec3(1, 1, 1);

void main() {
    vec3 albedo = vec3(1);
    float ambientStrength = 0.3;
    vec3 ambient = lightColor * ambientStrength;

    vec3 norm = normalize(inNormal);
    vec3 lightDir = normalize(lightPos - inFragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // float spec = pow(max(dot(inNormal, halfwayDir), 0.0), shininess);
    // vec3 specular = lightColor * spec;

    vec3 result = (ambient + diffuse) * albedo;
    outColor = vec4(result , 1.0);
}