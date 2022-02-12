vec3 phong(vec3 diffuseColor, vec3 specularColor, vec3 toLight, vec3 normal, vec3 toEye, float gloss) {
    vec3 L = normalize(toLight);
    vec3 N = normalize(normal);
    vec3 V = normalize(toEye);
    vec3 R = reflect(-L, N);

    float diffuseStrength = max(0.0, dot(L, N));
    float specularStrength = max(0.0, dot(R, V));
    specularStrength = pow(specularStrength, exp2(gloss * 16));

    return diffuseStrength * diffuseColor + specularStrength * specularColor;
}

vec3 blinn(vec3 diffuseColor, vec3 specularColor, vec3 toLight, vec3 normal, vec3 toEye, float gloss) {
    vec3 L = normalize(toLight);
    vec3 N = normalize(normal);
    vec3 V = normalize(toEye);
    vec3 H = normalize(L + V);

    float diffuseStrength = max(0.0, dot(L, N));
    float specularStrength = max(0.0, dot(N, H));
    specularStrength = pow(specularStrength, exp2(gloss * 16));

    return diffuseStrength * diffuseColor + specularStrength * specularColor / 3.0;
}

float fresnel(vec3 normal, vec3 toEye) {
    vec3 N = normalize(normal);
    vec3 V = normalize(toEye);

    return dot(N, V);
}
