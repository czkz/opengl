// Classic projection matrix
vec4 projx(vec3 v) {
    float fov = 90. * 3.1415 / 180;
    float near = 0.01;
    float far = 100.;
    float n = near;
    float f = far;

    float w = tan(fov/2.) * -v.z;
    v.z = (2*n*f/(f-n)/v.z + (f+n)/(f-n))*w;
    return vec4(v.xyz, w);
}
