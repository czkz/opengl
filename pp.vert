#version 330 core
layout (location = 0) in vec2 aPos;

out SHARED {
    vec2 st;
} _out;

void main() {
    _out.st = aPos * 0.5 + 0.5;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
