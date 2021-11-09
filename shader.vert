#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out SHARED {
    vec3 color;
} _out;

void main() {
    _out.color = aColor;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
