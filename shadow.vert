#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_M;
uniform mat4 u_shadowmap_VP;

void main() {
    vec4 p = vec4(aPos, 1.0);
    p = u_M * p;
    p = u_shadowmap_VP * p;
    gl_Position = p;
}