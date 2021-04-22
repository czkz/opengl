#version 330 core
layout (location = 0) in vec2 aPos;

out vec2 st;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    st = aPos / 2.0 + 0.5;
} 
