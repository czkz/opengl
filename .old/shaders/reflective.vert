#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
  
out vec3 sWorldPos;
out vec3 sColor;
out vec2 sTexCoord;
out vec3 sWorldNormal;

layout (std140) uniform Camera {
    uniform vec4 rotation;
    uniform vec3 position;
} camera;

uniform vec4 objectRotation;
uniform vec3 objectPosition;
uniform float objectScale;

##include linalg.glsl
##include projection.glsl

void main() {
    sWorldNormal = aNormal;
    sWorldNormal = qRotate(sWorldNormal, zUp2zBack(objectRotation));

    vec3 p = aPos;
    p = qRotate(p, zUp2zBack(objectRotation));
    p *= objectScale;
    p += zUp2zBack(objectPosition);
    sWorldPos = p;
    p -= zUp2zBack(camera.position);
    p = qRotate(p, zUp2zBack(qInverse(camera.rotation)));

    gl_Position = projx(p);
}
