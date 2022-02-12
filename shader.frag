#version 330 core
out vec4 FragColor;

in SHARED {
    vec3 posW;
    vec3 normalW;
    // vec2 st;
} _in;

uniform vec3 u_camera_world_pos;

uniform float u_time;
uniform sampler2D tex0;
uniform sampler2D tex1;

##include lighting.glsl

void main() {
    vec3 diffuseColor = texture(tex0, _in.posW.xy).rgb;
    // vec3 diffuseColor = vec3(0.8, 0.2, 0.05);
    vec3 specularColor = vec3(1, 1, 1);
    vec3 lightPos = vec3(0.3, -0.7, 1);

    vec3 c = phong(diffuseColor, specularColor, lightPos - _in.posW, _in.normalW, u_camera_world_pos - _in.posW, 0.5);

    FragColor = vec4(c, 1.0);
}
