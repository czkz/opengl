#version 330 core
out vec4 FragColor;

in SHARED {
    vec3 pos;
    vec3 normal;
    vec2 texCoord;
} _in;

layout (std140) uniform Camera {
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
uniform Material u_material;

##include linalg.glsl
##include lighting.glsl

void main() {
    vec3 normal = _in.normal;
    if (!gl_FrontFacing) {
        normal *= -1;
    }
    vec3 diffuse = texture(u_material.diffuse, _in.texCoord).rgb;
    vec3 specular = texture(u_material.specular, _in.texCoord).rgb;
    vec3 c = phong(_in.pos, lightPos, zUp2zBack(camera.position), normal,
                   diffuse, specular, u_material.shininess,
                   lightIntensity);
    FragColor = vec4(c, 1.0f);
}
