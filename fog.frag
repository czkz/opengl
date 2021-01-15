#version 330 core
out vec4 FragColor;

in vec4 pos;
in vec3 clr;
in vec2 texCoord;

uniform float uTime;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    float depth = length(pos.xyz) / 10.;
    depth = pow(depth, 2.);
    depth = smoothstep(0.5, 1.0, depth);
    vec4 texClr = texture(texture1, texCoord);
    vec4 texClr2 = texture(texture2, texCoord);
    vec3 c = mix(texClr.rgb, texClr2.rgb, 0.5);
    c = mix(c, vec3(1.), depth);
    FragColor = vec4(c, 1.0f);
}
