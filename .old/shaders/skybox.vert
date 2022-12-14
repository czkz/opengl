#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 sTexCoords;

layout (std140) uniform Camera {
    uniform vec4 rotation;
    uniform vec3 position;
} camera;

##include linalg.glsl

// Projection matrix with depth=1
vec4 proj_skybox(vec3 v) {
    float fov = 90. * 3.1415 / 180;

    float w = tan(fov/2.) * -v.z;
    return vec4(v.xy, w, w);
}

void main() {
    vec3 p = qRotate(aPos, zUp2zBack(camera.rotation));
    sTexCoords = p;
    gl_Position = proj_skybox(aPos);
} 
