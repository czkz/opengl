#version 330 core
out vec4 FragColor;

in SHARED {
    vec2 st;
} _in;

uniform sampler2D u_texture;

void main() {
    vec3 c = texture(u_texture, _in.st).rgb;

    // FragColor = vec4(dot(c, vec3(0.2126, 0.7152, 0.0722)) * vec3(1), 1.0);
    FragColor = vec4(c, 1.0);
}
