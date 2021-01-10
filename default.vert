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

float rand(vec2 pos) {
    return fract(sin(dot(pos, vec2(12.9898,78.233))) * 43758.5453123);
}

vec3 qRotate(vec3 p, vec4 q) {
    float qs = q.x;
    vec3 qv = q.yzw;
    float s12 = -dot(qv, p);
    vec3 v12 = p * qs + cross(qv, p);
    vec3 v3 = -qv;
    return v3 * s12 + v12 * qs + cross(v12, v3);
}

float foo(float x) {
    return x / (x + 2.0);
}

float bar(float x) {
    float e = exp(x);
    return e / (e + 1.0) * 2.0 - 1.0;
}

float simp(float x) {
    return x / (1000000.0 - 1.0);
}

// Classic projection matrix
vec4 projx(vec4 v) {
    float fov = 90. * 3.1415 / 180;
    float near = 0.01;
    float far = 10.;
    float n = near;
    float f = far;

    float w = tan(fov/2.) * -v.z;
    v.z = (2*n*f/(f-n)/v.z + (f+n)/(f-n))*w;
    return vec4(v.xyz, w);
}

// Somehow distance-based fov
vec4 projxxs(vec4 v) {
    float fov = 15. * 3.1415 / 180;
    float near = 0.01;
    float far = 10.;
    float n = near;
    float f = far;

    float pr = (-v.z) * (length(v.xyz) - near);
    float w = (fov/2.) * pr;
    // v.z = (-v.z - near) / (far - near) * w;
    v.z = (2*n*f/(f-n)/v.z + (f+n)/(f-n))*w;
    return vec4(v.xyz, w);
}


void main() {
    texCoord = aTexCoord;

    vec3 p = aPos;
    p += tr;
    p *= sc;
    p = qRotate(p, q);

    vec4 r = projx(vec4(p, 1.));
    gl_Position = r;

    // p.z -= 1;
    // float w = 1.0 + p.z * fov;
    // // p.z /= 2.;
    // p.z = simp(p.z);
    // p.z *= w;
    // gl_Position = vec4(p, w);


    // float w = 1. + (p.z * 1.);
    // p.xyz /= w;
    // // p.z *= bar(p.z);
    // // pos.z = 1.;
    // // float w = 1.;
    // clr = aColor;
    // pos = aPos;
    // // pos = vec4(p, w);
    // gl_Position = vec4(p.xy;
}
