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

// const vec3 lightPos = vec3(10, 10, 10);
const vec3 lightColor = vec3(1, 1, 1);
const float lightIntensity = 10;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material u_material;

uniform float u_time;

##include linalg.glsl
##include lighting.glsl

void main() {
    float time = u_time / 10.;
    vec3 lightPos = qRotate(vec3(10, 10, 10), vec4(cos(time) * vec3(0, 1, 0), sin(time)));

    vec3 diffuse = texture(u_material.diffuse, _in.texCoord).rgb;
    vec3 specular = texture(u_material.specular, _in.texCoord).rgb;
    vec3 c = phong(_in.pos, lightPos, zUp2zBack(camera.position), normalize(_in.normal),
                   diffuse, specular, u_material.shininess,
                   lightIntensity);
    FragColor = vec4(c, 1.0f);
}
