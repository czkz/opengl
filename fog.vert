#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
  
out vec4 pos;
out vec3 clr;
out vec2 texCoord;

uniform float uTime;

uniform vec4 q;
uniform vec3 tr;
uniform float sc;

vec3 qRotate(vec3 p, vec4 q) {
    float qs = q.x;
    vec3 qv = q.yzw;
    float s12 = -dot(qv, p);
    vec3 v12 = p * qs + cross(qv, p);
    vec3 v3 = -qv;
    return v3 * s12 + v12 * qs + cross(v12, v3);
}

float foo(float x) {
    return x / (x + 1.0);
}

void main() {
    vec3 p = qRotate(aPos, q);
    p += tr;
    p *= sc;
    float w = p.z * 1.;
    p.z *= foo(p.z);
    clr = aColor;
    texCoord = aTexCoord;
    pos = vec4(p, w);
    gl_Position = pos;
}
