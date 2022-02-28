#version 330 core

in SHARED {
    vec4 posW;
} _in;

uniform float u_shadowmapFarPlane;
uniform struct {
    vec3 pos;
} u_light;

void main() {
    float lightDistance = length(_in.posW.xyz - u_light.pos);
    gl_FragDepth = lightDistance / u_shadowmapFarPlane;
}
