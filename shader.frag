#version 330 core
out vec4 FragColor;

in SHARED {
    vec3 posW;
    vec3 normalW;
    vec2 st;
} _in;

uniform vec3 u_camera_world_pos;

uniform float u_time;
uniform sampler2D tex0;
uniform sampler2D tex1;

void main() {
    float gloss = 0.5;

    vec2 t = vec2(cos(u_time), sin(u_time));
    vec3 diffuseColor = texture(tex0, _in.st).rgb;
    diffuseColor *= texture(tex1, _in.st).rgb;

    vec3 L = normalize(vec3(0.3, -0.7, 1));
    vec3 N = normalize(_in.normalW);
    vec3 V = normalize(u_camera_world_pos - _in.posW);
    vec3 R = reflect(-L, N);
    float diffuseStrength = max(0.0, dot(L, N));
    float specularStrength = max(0.0, dot(R, V));
    specularStrength = pow(specularStrength, exp2(gloss * 16)) * gloss;
    vec3 specularColor = vec3(1, 1, 1);

    vec3 c = diffuseStrength * diffuseColor + specularStrength * specularColor;
    FragColor = vec4(c, 1.0);
}
