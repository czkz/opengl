#version 330 core
out vec4 FragColor;

in SHARED {
    vec3 color;
} _in;

uniform float u_time;

void main() {
    float t = abs(sin(u_time));
    FragColor = vec4(_in.color * t, 1.0);
}
