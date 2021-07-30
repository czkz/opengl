#version 330 core
out vec4 FragColor;

in SHARED {
    vec3 pos;
} _in;

layout (std140) uniform Camera {
    uniform vec4 rotation;
    uniform vec3 position;
} camera;

##include linalg.glsl

void main() {
    vec3 c = vec3(50.0 / distance(_in.pos, zUp2zBack(camera.position)));
    FragColor = vec4(c, 1.0f);
}
