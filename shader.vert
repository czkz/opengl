#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out SHARED {
    vec3 normal;
    vec2 st;
} _out;

uniform mat4 u_camera;
uniform mat4 u_transform;
uniform mat4 u_projection;

void main() {
    _out.st = aTexCoord;
    _out.normal = aNormal;
    vec4 p = vec4(aPos, 1.0);
    p = u_transform * p;
    p = u_camera * p;
    p = u_projection * p;
    gl_Position = p;
}
