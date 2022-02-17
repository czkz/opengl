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

uniform mat4 u_camera;
uniform bool u_is_orthographic;

##include lighting.glsl

void main() {
    vec3 diffuseColor = texture(tex0, _in.posW.xy).rgb;
    // vec3 diffuseColor = vec3(0.8, 0.2, 0.05);
    vec3 specularColor = u_light_color;
    vec3 lightPos = u_light_pos;
    vec3 toLight = lightPos - _in.posW;
    vec3 toEye;
    if (u_is_orthographic) {
        toEye = (inverse(u_camera) * vec4(0.0, -1.0, 0.0, 0.0)).xyz;
    } else {
        toEye = u_camera_world_pos - _in.posW;
    }
    float lightDist = length(toLight);
    float attenuation = 1.0 / (lightDist * lightDist);

    vec3 c = u_light_intensity * attenuation * phong(
        diffuseColor,
        specularColor,
        toLight,
        _in.normalW,
        toEye,
        0.5
    );
    c += diffuseColor * 0.01;

    FragColor = vec4(c, 1.0);
}
