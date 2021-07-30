#version 330 core
layout (points) in;
layout (points, max_vertices = 1) out;

in SHARED {
    vec3 pos;
} _in[];

out SHARED {
    vec3 pos;
} _out;

void main() {
    for (int i = 0; i < 1; i++) {
        gl_Position = gl_in[i].gl_Position;
        _out.pos = _in[i].pos;
        EmitVertex();
    }
    EndPrimitive();
}
