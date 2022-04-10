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
} _out;

uniform mat4 u_M;
uniform mat4 u_V;
uniform mat4 u_P;

void main() {
    _out.st = aTexCoords;

    _out.tan2world = transpose(inverse(mat3(u_M))) * mat3(aTangent, aBitangent, aNormal);
    _out.tan2world[0] = normalize(_out.tan2world[0]);
    _out.tan2world[1] = normalize(_out.tan2world[1]);
    _out.tan2world[2] = normalize(_out.tan2world[2]);

    vec4 p = vec4(aPos, 1.0);
    p = u_M * p;
    _out.posW = p.xyz;
    p = u_V * p;
    p = u_P * p;
    gl_Position = p;
}
