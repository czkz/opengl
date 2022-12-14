#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
  
out SHARED {
    vec3 pos;
    vec3 normal;
    vec2 texCoord;
} _out;

layout (std140) uniform Camera {
    uniform vec4 rotation;
    uniform vec3 position;
} camera;

layout (std140) uniform Transform {
    vec4 rotation;
    vec3 position;
    float scale;
} u_object;

##include linalg.glsl


void main() {
    _out.texCoord = aTexCoord;
    _out.normal = qRotate(aNormal, zUp2zBack(u_object.rotation));

    vec3 p = aPos;
    p = qRotate(p, zUp2zBack(u_object.rotation));
    p *= u_object.scale;
    p += zUp2zBack(u_object.position);
    _out.pos = p;
    p -= zUp2zBack(camera.position);
    p = qRotate(p, zUp2zBack(qInverse(camera.rotation)));

    gl_Position = vec4(p, 1.0);
}
