#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
  
out vec3 pos;
out vec3 clr;

uniform float uTime;

float rand(vec2 pos) {
    return fract(sin(dot(pos, vec2(12.9898,78.233))) * 43758.5453123);
}

void main() {
    float o = uTime + rand(aPos.xy) * 123.456;
    pos = aPos + vec3(sin(o), cos(o), 0) * 0.1;
    clr = aColor;
    gl_Position = vec4(pos, 1.0);
}
