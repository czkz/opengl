vec3 phong(
    vec3 diffuseColor,
    vec3 specularColor,
    vec3 toLightDir,
    vec3 normalDir,
    vec3 toEyeDir,
    float gloss
) {
    vec3 L = toLightDir;
    vec3 N = normalDir;
    vec3 V = toEyeDir;
    vec3 R = reflect(-L, N);

    float diffuseStrength = max(0.0, dot(L, N));
    float specularStrength = max(0.0, dot(R, V));
    specularStrength = pow(specularStrength, exp2(gloss * 16));

    return diffuseStrength * diffuseColor + specularStrength * specularColor;
}

vec3 blinn(
    vec3 diffuseColor,
    vec3 specularColor,
    vec3 toLightDir,
    vec3 normalDir,
    vec3 toEyeDir,
    float gloss
) {
    vec3 L = toLightDir;
    vec3 N = normalDir;
    vec3 V = toEyeDir;
    vec3 H = normalize(L + V);

    float diffuseStrength = max(0.0, dot(L, N));
    float specularStrength = max(0.0, dot(N, H));
    specularStrength = pow(specularStrength, exp2(gloss * 16));

    return diffuseStrength * diffuseColor + specularStrength * specularColor / 3.0;
}

float fresnel(vec3 normalDir, vec3 toEyeDir) {
    vec3 N = normalDir;
    vec3 V = toEyeDir;

    return dot(N, V);
}
