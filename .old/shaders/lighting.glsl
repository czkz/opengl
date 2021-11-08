float attenuation(float d) {
    const float c = 1.0;
    const float l = 0.0;
    const float q = 0.25;

    return 1.0 / (c + l*d + q*d*d);
}

// Actually Blinn-Phong
vec3 phong(vec3 pos, vec3 lightPos, vec3 cameraPos, vec3 normalDir,
           vec3 diffuse, vec3 specular, float shininess,
           float lightIntensity)
{
    vec3 toLight = lightPos - pos;

    float ambientStrengthMod = 0.05;
    float ambientStrength = ambientStrengthMod;
    vec3 ambientColor = ambientStrength * diffuse;

    vec3 toLightDir = normalize(toLight);
    float diffuseStrength = max(0.0, dot(toLightDir, normalDir));
    vec3 diffuseColor = diffuse * diffuseStrength;

    vec3 toEyeDir = normalize(cameraPos - pos);
    vec3 halfwayDir = normalize(toEyeDir + toLightDir);
    float specularStrength = pow(max(0.0, dot(normalDir, halfwayDir)), shininess);
    vec3 specularColor = specularStrength * specular;

    vec3 c = ambientColor + diffuseColor + specularColor;

    float distMod = attenuation(length(toLight) / lightIntensity);
    return c * distMod;
}
