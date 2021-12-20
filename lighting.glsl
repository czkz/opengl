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
