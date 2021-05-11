float attenuation(float d) {
    const float c = 1.0;
    const float l = 0.045;
    const float q = 0.0075;

    return 1.0 / (c + l*d + q*d*d);
}

vec3 phong(vec3 pos, vec3 lightPos, vec3 cameraPos, vec3 normal,
           vec3 diffuse, vec3 specular, float shininess,
           float lightIntensity)
{
    vec3 toLight = lightPos - pos;
    float distMod = attenuation(length(toLight) / lightIntensity);

    float ambientStrengthMod = 0.1;
    float ambientStrength = ambientStrengthMod * distMod;
    vec3 ambientColor = ambientStrength * diffuse;

    vec3 toLightDir = normalize(toLight);
    float diffuseStrength = max(0.0, dot(toLightDir, normal)) * distMod;
    vec3 diffuseColor = diffuse * diffuseStrength;

    float specularStrengthMod = 1.0;
    vec3 toEyeDir = normalize(cameraPos - pos);
    vec3 reflectDir = reflect(-toLightDir, normal);
    float specularStrength = specularStrengthMod * pow(max(0.0, dot(reflectDir, toEyeDir)), shininess) * distMod;
    vec3 specularColor = specularStrength * specular;

    vec3 c = ambientColor + diffuseColor + specularColor;

    return c;
}
