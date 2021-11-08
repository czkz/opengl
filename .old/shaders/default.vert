#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in vec3 position;
layout (location = 4) in vec4 rotation;
layout (location = 5) in float scale;
  
out SHARED {
    vec3 pos;
    vec3 normal;
    vec2 texCoord;
} _out;

layout (std140) uniform Camera {
    uniform vec4 rotation;
    uniform vec3 position;
} camera;

##include linalg.glsl
##include projection.glsl

void main() {
    _out.texCoord = aTexCoord;
    _out.normal = qRotate(aNormal, zUp2zBack(rotation));

    vec3 p = aPos;
    p = qRotate(p, zUp2zBack(rotation));
    p *= scale;
    p += zUp2zBack(position);
    _out.pos = p;
    p -= zUp2zBack(camera.position);
    p = qRotate(p, zUp2zBack(qInverse(camera.rotation)));

    gl_Position = projx(p);
}
