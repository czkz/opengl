#version 330 core
out vec4 FragColor;

uniform float u_time;

void main() {
    FragColor = vec4(sin(u_time)*.5+.5, 0.0, 0.0, 1.0);
}
