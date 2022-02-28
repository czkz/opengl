#version 330 core
out vec4 FragColor;

in SHARED {
    vec3 posM;
} _in;

uniform samplerCube u_shadowmap;

void main() {
    float f = texture(u_shadowmap, _in.posM).r;

    vec3 c = vec3(f);
    FragColor = vec4(c, 1.0);
}
