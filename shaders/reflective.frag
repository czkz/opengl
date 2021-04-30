#version 330 core
out vec4 FragColor;

in vec3 sWorldPos;
in vec2 sTexCoord;
in vec3 sWorldNormal;

uniform samplerCube skybox;

layout (std140) uniform CAMERA {
    uniform vec4 rotation;
    uniform vec3 position;
} camera;


vec3 zUp2zBack(vec3 p) {
    return vec3(p.x, p.z, -p.y);
}

void main() {
    vec3 I = normalize(sWorldPos - zUp2zBack(camera.position));
    vec3 R = reflect(I, normalize(sWorldNormal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}