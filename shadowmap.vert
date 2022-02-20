#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_camera;
uniform mat4 u_transform;
uniform mat4 u_projection;

void main() {
    vec4 p = vec4(aPos, 1.0);
    p = u_transform * p;
    p = u_camera * p;
    p = u_projection * p;
    gl_Position = p;
}
