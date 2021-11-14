#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out SHARED {
    vec2 st;
} _out;

uniform mat4 u_transform;

void main() {
    _out.st = aTexCoord;
    vec4 p = vec4(aPos, 0.0, 1.0);
    p = u_transform * p;
    gl_Position = p;
}
