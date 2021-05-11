#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
  
out SHARED {
    vec3 pos;
    vec3 normal;
    vec2 texCoord;
} _out;

uniform float uTime;

layout (std140) uniform CAMERA {
    uniform vec4 rotation;
    uniform vec3 position;
} camera;

struct Object {
    vec4 rotation;
    vec3 position;
    float scale;
};
uniform Object u_object;

##include linalg.glsl
##include projection.glsl

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

    gl_Position = projx(p);
}
