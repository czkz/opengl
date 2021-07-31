#version 330 core

out SHARED {
    vec3 pos;
} _out;

layout (std140) uniform Camera {
    uniform vec4 rotation;
    uniform vec3 position;
} camera;

uniform float u_time;

##include linalg.glsl
##include projection.glsl
##include noise4D.glsl

const float pi = 3.1415926535897932384626433833;
const float phi = 1.618033988749894848204586834;

void main() {
    float si = sqrt(gl_InstanceID);
    float halfang = fract(si * phi) * pi;
    vec4 rotation1 = vec4(vec3(0.0, 0.0, 1.0) * sin(halfang), cos(halfang));
    vec4 rotation2 = vec4(vec3(0.0, 1.0, 0.0) * sin(halfang*pi), cos(halfang*pi));
    vec3 position = vec3(si, 0.0, 0.0);
    position = qRotate(position, rotation1);
    position = qRotate(position, rotation2);

    vec4 rotation3 = vec4(
        snoise(vec4(position*1.0, 10.)),
        snoise(vec4(position*1.0, 20.)),
        snoise(vec4(position*1.0, 30.)),
        snoise(vec4(position*1.0, 40.))
    );
    position = qRotate(position, rotation3);

    vec3 p = zUp2zBack(position);
    _out.pos = p;
    p -= zUp2zBack(camera.position);
    p = qRotate(p, zUp2zBack(qInverse(camera.rotation)));

    gl_Position = projx(p);
}
