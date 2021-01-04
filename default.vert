#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
  
out vec3 pos;
out vec3 clr;
out vec2 texCoord;

uniform float uTime;

float rand(vec2 pos) {
    return fract(sin(dot(pos, vec2(12.9898,78.233))) * 43758.5453123);
}

void main() {
    // float o = uTime + rand(aPos.xy) * 123.456;
    // pos = (aPos + vec3(sin(o), cos(o), 0) * 0.1); /[> (cos(uTime * 0.5) * 0.5 + 0.5);
    pos = aPos;
    clr = aColor;
    texCoord = aTexCoord;
    gl_Position = vec4(pos, 1.0);
}
