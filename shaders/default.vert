#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
  
out vec3 sPos;
out vec3 sColor;
out vec2 sTexCoord;

uniform float uTime;

uniform vec4 cameraRotation;
uniform vec3 cameraPosition;

uniform vec4 objectRotation;
uniform vec3 objectPosition;
uniform float objectScale;

float rand(vec2 pos) {
    return fract(sin(dot(pos, vec2(12.9898,78.233))) * 43758.5453123);
}

vec3 zUp2zBack(vec3 p) {
    return vec3(p.x, p.z, -p.y);
}

vec4 zUp2zBack(vec4 q) {
    return vec4(q.x, q.z, -q.y, q.w);
}

vec3 qRotate(vec3 p, vec4 q) {
    float qs = q.w;
    vec3 qv = q.xyz;
    float s12 = -dot(qv, p);
    vec3 v12 = p * qs + cross(qv, p);
    vec3 v3 = -qv;
    return v3 * s12 + v12 * qs + cross(v12, v3);
}

vec4 qInverse(vec4 q) {
    return vec4(-q.xyz, q.w);
}

// Classic projection matrix
vec4 projx(vec4 v) {
    float fov = 90. * 3.1415 / 180;
    float near = 0.01;
    float far = 100.;
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
    float far = 100.;
    float n = near;
    float f = far;

    float pr = (-v.z) * (length(v.xyz) - near);
    float w = (fov/2.) * pr;
    // v.z = (-v.z - near) / (far - near) * w;
    v.z = (2*n*f/(f-n)/v.z + (f+n)/(f-n))*w;
    return vec4(v.xyz, w);
}


void main() {
    sTexCoord = aTexCoord;

    vec3 p = aPos;
    p = qRotate(p, zUp2zBack(objectRotation));
    p *= objectScale;
    p += zUp2zBack(objectPosition);
    p -= zUp2zBack(cameraPosition);
    p = qRotate(p, zUp2zBack(qInverse(cameraRotation)));
    sPos = p;

    gl_Position = projx(vec4(p, 1.));
}
