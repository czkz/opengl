#version 330 core
layout (location = 0) in vec3 aPos;

out SHARED {
    vec3 posM;
} _out;

uniform mat4 u_M;
uniform mat4 u_V;
uniform mat4 u_P;

void main() {
    _out.posM = aPos;
    vec4 p = vec4(aPos, 1.0);
    p = u_P * u_V * u_M * p;
    gl_Position = p;
}
