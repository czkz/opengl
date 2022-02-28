#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 u_V[6];
uniform mat4 u_P;

out SHARED {
    vec4 posW;
} _out;

void main() {
    for (int face_i = 0; face_i < 6; face_i++) {
        gl_Layer = face_i;
        for (int i = 0; i < 3; i++) {
            vec4 p = gl_in[i].gl_Position;
            _out.posW = p;
            p = u_P * u_V[face_i] * p;
            gl_Position = p;
            EmitVertex();
        }
        EndPrimitive();
    }
}
