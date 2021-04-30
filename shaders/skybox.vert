#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 sTexCoords;

layout (std140) uniform CAMERA {
    uniform vec4 rotation;
    uniform vec3 position;
} camera;


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
