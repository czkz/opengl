#version 330 core
out vec4 FragColor;

in vec3 sPos;
in vec3 sNormal;
in vec2 sTexCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 cameraPosition;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
uniform Material material;

vec3 zUp2zBack(vec3 p) {
    return vec3(p.x, p.z, -p.y);
}

vec3 phong(vec3 pos, vec3 lightPos, vec3 cameraPos, vec3 normal, vec3 objectColor, vec3 lightColor) {
    float ambientStrength = 0.1;
    vec3 ambientColor = ambientStrength * objectColor;

    vec3 toLightDir = normalize(lightPos - pos);
    float diffuseStrength = max(0.0, dot(toLightDir, normal));
    vec3 diffuseColor = objectColor * diffuseStrength;

    float specularStrengthMod = 1.0;
    vec3 toEyeDir = normalize(cameraPos - pos);
    vec3 reflectDir = reflect(-toLightDir, normal);
    float specularStrength = specularStrengthMod * pow(max(0.0, dot(reflectDir, toEyeDir)), material.shininess);
    vec3 specularColor = specularStrength * texture(material.specular, sTexCoord).rgb;

    vec3 emissionColor = texture(material.emission, sTexCoord).rgb;

    vec3 c = ambientColor + diffuseColor + specularColor;

    return c + emissionColor * ( 1. - smoothstep(0.0, 0.3, diffuseStrength + specularStrength) );
}

void main() {
    vec3 objectColor = texture(material.diffuse, sTexCoord).rgb;
    vec3 phongColor = phong(sPos, zUp2zBack(lightPos), zUp2zBack(cameraPosition), sNormal, objectColor, lightColor);
    FragColor = vec4(phongColor, 1.0f);
}
