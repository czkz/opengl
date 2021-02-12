#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform float lightIntensity;

void main() {
    FragColor = vec4(lightColor * lightIntensity / 5, 1.0f);
}
