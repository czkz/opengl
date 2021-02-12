#version 330 core
out vec4 FragColor;

in vec3 sPos;
in vec3 sNormal;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 cameraPosition;

vec3 zUp2zBack(vec3 p) {
    return vec3(p.x, p.z, -p.y);
}

vec3 phong(vec3 pos, vec3 lightPos, vec3 cameraPos, vec3 normal, vec3 objectColor, vec3 lightColor) {
    float ambientStrength = 0.1;
    vec3 ambientColor = ambientStrength * lightColor;

    vec3 toLightDir = normalize(lightPos - pos);
    float diffuseStrength = max(0.0, dot(toLightDir, normal));
    vec3 diffuseColor = lightColor * diffuseStrength;

    float specularStrength = 0.5;
    vec3 toEyeDir = normalize(cameraPos - pos);
    vec3 reflectDir = reflect(-toLightDir, normal);
    vec3 specularColor = specularStrength * lightColor * pow(max(0.0, dot(reflectDir, toEyeDir)), 32);

    return objectColor * (ambientColor + diffuseColor + specularColor);
}

void main() {
    vec3 objectColor = vec3(0.2, 0.5, 0.7);
    vec3 phongColor = phong(sPos, zUp2zBack(lightPos), zUp2zBack(cameraPosition), sNormal, objectColor, lightColor);
    FragColor = vec4(phongColor, 1.0f);
}
