#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_M;

void main() {
    vec4 p = vec4(aPos, 1.0);
    p = u_M * p;
    gl_Position = p;
}
