#version 330 core
out vec4 FragColor;

in SHARED {
    vec3 normal;
    vec2 st;
} _in;

uniform float u_time;
uniform sampler2D tex0;
uniform sampler2D tex1;

void main() {
    float t = abs(sin(u_time));
    vec4 c = texture(tex0, _in.st);
    c *= texture(tex1, _in.st);
    FragColor = vec4(c.rgb * t, 1.0);
}
