#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out SHARED {
    vec2 st;
} _out;

uniform mat4 u_camera;
uniform mat4 u_transform;

const float fov = 90. * 3.1415 / 180.0;
const float aspect = 1;
const float n = 0.01;
const float f = 100.;
const float ff = 1.0/tan(fov/2.0);
const mat4 projectionMatrix = transpose(mat4(
    ff / aspect, 0,  0,            0,
    0,           ff, 0,            0,
    0,           0,  -(f+n)/(f-n), -2.0*f*n/(f-n),
    0,           0,  -1,           0
));

vec4 zUp2zBack(vec4 p) {
    return vec4(p.x, p.z, -p.y, p.w);
}

void main() {
    _out.st = aTexCoord;
    vec4 p = vec4(aPos, 0.0, 1.0);
    p = u_transform * p;
    p = u_camera * p;
    p = zUp2zBack(p);
    p = projectionMatrix * p;
    gl_Position = p;
}
