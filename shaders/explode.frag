#version 330 core
out vec4 FragColor;

in SHARED {
    vec3 pos;
    vec3 normal;
    vec2 texCoord;
} _in;

uniform float uTime;

layout (std140) uniform CAMERA {
    uniform vec4 rotation;
    uniform vec3 position;
} camera;

const vec3 lightPos = vec3(10, 10, 10);
const vec3 lightColor = vec3(1, 1, 1);
const float lightIntensity = 10;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

##include linalg.glsl

float attenuation(float d) {
    const float c = 1.0;
    const float l = 0.045;
    const float q = 0.0075;

    return 1.0 / (c + l*d + q*d*d);
}

vec3 phong(vec3 pos, vec3 lightPos, vec3 cameraPos, vec3 normal, vec3 objectColor) {
    vec3 toLight = lightPos - pos;
    float distMod = attenuation(length(toLight) / lightIntensity);

    float ambientStrengthMod = 0.1;
    float ambientStrength = ambientStrengthMod * distMod;
    vec3 ambientColor = ambientStrength * objectColor;

    vec3 toLightDir = normalize(toLight);
    float diffuseStrength = max(0.0, dot(toLightDir, normal)) * distMod;
    vec3 diffuseColor = objectColor * diffuseStrength;

    float specularStrengthMod = 1.0;
    vec3 toEyeDir = normalize(cameraPos - pos);
    vec3 reflectDir = reflect(-toLightDir, normal);
    float specularStrength = specularStrengthMod * pow(max(0.0, dot(reflectDir, toEyeDir)), material.shininess) * distMod;
    vec3 specularColor = specularStrength * texture(material.specular, _in.texCoord).rgb;

    vec3 c = ambientColor + diffuseColor + specularColor;

    return c;
}

void main() {
    vec3 normal = _in.normal;
    if (!gl_FrontFacing) {
        normal *= -1;
    }
    vec3 diffuseColor = texture(material.diffuse, _in.texCoord).rgb;
    vec3 c = phong(_in.pos, lightPos, zUp2zBack(camera.position), normal, diffuseColor);
    FragColor = vec4(c, 1.0f);
}
