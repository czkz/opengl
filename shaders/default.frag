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
##include projection.glsl

void main() {
    float f = distance(_in.pos, zUp2zBack(camera.position));
    f = pow(f / projx_far, 0.5);
    vec3 c = vec3(1.0 - f);
    FragColor = vec4(c, 1.0f);
}
