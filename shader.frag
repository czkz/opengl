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

uniform vec3 u_light_pos;
uniform vec3 u_light_color;
uniform float u_light_intensity;

##include lighting.glsl

void main() {
    vec3 diffuseColor = texture(tex0, _in.posW.xy).rgb;
    // vec3 diffuseColor = vec3(0.8, 0.2, 0.05);
    vec3 specularColor = u_light_color;
    vec3 lightPos = u_light_pos;
    vec3 toLight = lightPos - _in.posW;
    float lightDist = length(toLight);
    float attenuation = 1.0 / (lightDist * lightDist);

    vec3 c = u_light_intensity * attenuation * phong(diffuseColor, specularColor, toLight, _in.normalW, u_camera_world_pos - _in.posW, 0.5);

    FragColor = vec4(c, 1.0);
}
