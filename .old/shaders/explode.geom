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

uniform float u_time;

##include projection.glsl

vec3 get_normal() {
    vec3 v0 = gl_in[0].gl_Position.xyz;
    vec3 v1 = gl_in[1].gl_Position.xyz;
    vec3 v2 = gl_in[2].gl_Position.xyz;
    vec3 a = v2 - v1;
    vec3 b = v0 - v1;
    return normalize(cross(a, b));
}

vec3 explode(vec3 position, vec3 normal) {
    float magnitude = 1.0 * (sin(u_time) / 2.0 + 0.5);
    return position + normal * magnitude;
}

void main() {
    vec3 n = get_normal();

    for (int i = 0; i < 3; i++) {
        vec3 p = explode(gl_in[i].gl_Position.xyz, n);
        gl_Position = projx(p);
        _out.pos = explode(_in[i].pos, _in[i].normal);
        _out.normal = _in[i].normal;
        _out.texCoord = _in[i].texCoord;
        EmitVertex();
    }
    EndPrimitive();
}
