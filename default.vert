#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
  
out vec3 pos;
out vec3 clr;
out vec2 texCoord;

uniform float uTime;

uniform float qs;
uniform vec3 qv;

float rand(vec2 pos) {
    return fract(sin(dot(pos, vec2(12.9898,78.233))) * 43758.5453123);
}

vec3 qRotate(vec3 p, float qs, vec3 qv) {
    float s12 = -dot(qv, p);
    vec3 v12 = p * qs + cross(qv, p);
    vec3 v3 = -qv;
    return v3 * s12 + v12 * qs + cross(v12, v3);
}

void main() {
    pos = qRotate(aPos, qs, qv);
    clr = aColor;
    texCoord = aTexCoord;
    gl_Position = vec4(pos, 1.0);
}
