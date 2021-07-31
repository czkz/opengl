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

    // position.z += sin(u_time + gl_InstanceID * 0.1);

    // position.x += snoise(vec4(position*0.5, (u_time + 100.0)*0.1));
    // position.y += snoise(vec4(position*0.5, (u_time + 200.0)*0.1));
    // position.z += snoise(vec4(position*0.5, (u_time + 300.0)*0.1));

    vec4 rotation3;
    rotation3.x += snoise(vec4(position*0.01, (u_time + 100.0)*0.001));
    rotation3.y += snoise(vec4(position*0.01, (u_time + 200.0)*0.001));
    rotation3.z += snoise(vec4(position*0.01, (u_time + 300.0)*0.001));
    rotation3.w += snoise(vec4(position*0.01, (u_time + 400.0)*0.001));
    position = qRotate(position, rotation3);
    position = qRotate(position, rotation1);
    position = qRotate(position, rotation2);

    // p = qRotate(p, zUp2zBack(rotation));
    // p *= scale;
    vec3 p = zUp2zBack(position);
    _out.pos = p;
    p -= zUp2zBack(camera.position);
    p = qRotate(p, zUp2zBack(qInverse(camera.rotation)));

    gl_Position = projx(p);
}
