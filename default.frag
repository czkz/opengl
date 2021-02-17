#version 330 core
out vec4 FragColor;

in vec3 sPos;
in vec3 sColor;
in vec2 sTexCoord;

uniform float uTime;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float colorScale;


float line(float d) {
    const float c = 0.001;
    const float l = 0.01;
    return smoothstep(-l, -(l-c), d) - smoothstep(l-c, l, d);
}

void main() {
    vec4 texClr = texture(texture1, sTexCoord);
    vec4 texClr2 = texture(texture2, sTexCoord);
    vec3 c = mix(texClr.rgb, texClr2.rgb, 0.5);
    c *= colorScale;
    FragColor = vec4(c, 1.0f);
}
