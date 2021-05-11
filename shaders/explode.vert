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

uniform vec4 objectRotation;
uniform vec3 objectPosition;
uniform float objectScale;

##include linalg.glsl

void main() {
    _out.texCoord = aTexCoord;
    _out.normal = qRotate(aNormal, zUp2zBack(objectRotation));

    vec3 p = aPos;
    p = qRotate(p, zUp2zBack(objectRotation));
    p *= objectScale;
    p += zUp2zBack(objectPosition);
    _out.pos = p;
    p -= zUp2zBack(camera.position);
    p = qRotate(p, zUp2zBack(qInverse(camera.rotation)));

    gl_Position = vec4(p, 1.0);
}
