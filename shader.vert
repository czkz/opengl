#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out SHARED {
    vec3 posW;
    vec2 st;
    mat3 tan2world;
    vec3 posM;
} _out;

uniform mat4 u_M;
uniform mat4 u_V;
uniform mat4 u_P;

void main() {
    _out.st = aTexCoords;

    mat3 normalMatrix = mat3(transpose(inverse(u_M)));
    mat3 TBN = mat3(aTangent, aBitangent, aNormal);
    _out.tan2world = normalMatrix * TBN;

    vec4 p = vec4(aPos, 1.0);
    _out.posM = p.xyz;
    p = u_M * p;
    _out.posW = p.xyz;
    p = u_V * p;
    p = u_P * p;
    gl_Position = p;
}
