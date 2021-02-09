
#version 330 core
out vec4 FragColor;

in vec3 sPos;
in vec3 sColor;
in vec2 sTexCoord;

uniform float uTime;
uniform sampler2D texture1;
uniform sampler2D texture2;


void main() {
    float depth = length(sPos) / 10.;
    depth = pow(depth, 2.);
    depth = smoothstep(0.5, 1.0, depth);

    vec4 texClr = texture(texture1, sTexCoord);
    vec4 texClr2 = texture(texture2, sTexCoord);
    vec3 c = mix(texClr.rgb, texClr2.rgb, 0.5);
    c = mix(c, sColor, 0.25);
    c = mix(c, vec3(1.), depth);
    FragColor = vec4(c, 1.0f);
}
