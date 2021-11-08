vec3 zUp2zBack(vec3 p) {
    return vec3(p.x, p.z, -p.y);
}

vec4 zUp2zBack(vec4 q) {
    return vec4(q.x, q.z, -q.y, q.w);
}

vec3 qRotate(vec3 p, vec4 q) {
    float qs = q.w;
    vec3 qv = q.xyz;
    float s12 = -dot(qv, p);
    vec3 v12 = p * qs + cross(qv, p);
    vec3 v3 = -qv;
    return v3 * s12 + v12 * qs + cross(v12, v3);
}

vec4 qInverse(vec4 q) {
    return vec4(-q.xyz, q.w);
}

