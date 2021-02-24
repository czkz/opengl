#version 330 core
layout (location = 0) in vec2 aTexCoords;

out vec2 sTexCoords;

void main() {
    vec2 pos = aTexCoords * 2. - 1.;
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0); 
    sTexCoords = aTexCoords;
}
