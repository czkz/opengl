#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aTexCoord;

out SHARED {
    vec3 posW;
    vec3 normalW;
    // vec2 st;
} _out;

uniform mat4 u_camera;
uniform mat4 u_transform;
uniform mat4 u_projection;

void main() {
    // _out.st = aTexCoord;
    // _out.normalW = (u_transform * vec4(aNormal, 0.0)).xyz;
    _out.normalW = (u_transform * vec4(aPos, 0.0)).xyz;
    vec4 p = vec4(aPos, 1.0);
    p.xyz = normalize(p.xyz);
    p = u_transform * p;
    _out.posW = p.xyz;
    p = u_camera * p;
    p = u_projection * p;
    gl_Position = p;
}
