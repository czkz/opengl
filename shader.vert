#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aTexCoord;

out SHARED {
    vec3 posW;
    vec3 normalW;
    // vec2 st;
    vec4 shadowmapFragCoord;
} _out;

uniform mat4 u_M;
uniform mat4 u_V;
uniform mat4 u_P;

uniform mat4 u_shadowmap_VP;

void main() {
    // _out.st = aTexCoord;
    _out.normalW = (u_M * vec4(aNormal, 0.0)).xyz;
    vec4 p = vec4(aPos, 1.0);
    p = u_M * p;
    _out.posW = p.xyz;
    _out.shadowmapFragCoord = u_shadowmap_VP * p;
    p = u_V * p;
    p = u_P * p;
    gl_Position = p;
}
