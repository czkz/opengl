#version 330 core
out vec4 FragColor;

in SHARED {
    vec2 st;
} _in;

uniform sampler2D u_texture;
uniform float u_exposure;

void main() {
    vec3 c = texture(u_texture, _in.st).rgb;
    c = 1.0 - exp(-c * u_exposure);

    // FragColor = vec4(dot(c, vec3(0.2126, 0.7152, 0.0722)) * vec3(1), 1.0);
    FragColor = vec4(c, 1.0);
}
