#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in SHARED {
    vec3 pos;
    vec3 normal;
    vec2 texCoord;
} _in[];

out SHARED {
    vec3 pos;
    vec3 normal;
    vec2 texCoord;
} _out;

void main() {
    for (int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        _out.pos = _in[i].pos;
        _out.normal = _in[i].normal;
        _out.texCoord = _in[i].texCoord;
        EmitVertex();
    }
    EndPrimitive();
}
