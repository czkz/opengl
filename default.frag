#version 330 core
out vec4 FragColor;

in vec3 pos;
in vec3 clr;

uniform float uTime;

void main() {
    FragColor = vec4(clr.rgb, 1.0f);
    // FragColor = vec4(sin(pos.x + pos.y + uTime), cos(pos.x + pos.y + uTime), 1.0f, 1.0f);
} 
