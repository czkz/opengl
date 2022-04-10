#version 330 core
out vec4 FragColor;

in SHARED {
    vec3 posW;
    vec2 st;
    mat3 tan2world;
} _in;

uniform float u_time;
uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_depthMap;

#define MAX_LIGHTS 8
uniform struct {
    vec3 pos;
    vec3 color;
    float intensity;
} u_lights[MAX_LIGHTS];
uniform int u_nLights;

uniform mat4 u_V;
uniform bool u_isOrthographic;

// uniform float u_shadowmapFarPlane;
// uniform samplerCube u_shadowmap;

##include lighting.glsl

vec3 parallaxMap(vec3 toEyeDirT, vec2 st, sampler2D depthMap,
                     float numLayersMin, float numLayersMax)
{
    const float heightScale = 0.05;
    float numLayers = mix(numLayersMax, numLayersMin, dot(vec3(0,0,1), toEyeDirT));
    if (numLayers > 100) { discard; }
    vec3 sampleStep = -toEyeDirT.xyz / toEyeDirT.z * heightScale / numLayers;

    vec3 currentPos = vec3(st, 0.0);

    int i = 0;
    while(currentPos.z > -texture(depthMap, currentPos.xy).r * heightScale) {
        if (++i > 100) { discard; break; }
        currentPos += sampleStep;
    }

    vec3 p0 = currentPos - sampleStep;
    vec3 p1 = currentPos;
    float err0 = p0.z - -texture(depthMap, p0.xy).r * heightScale;
    float err1 = p1.z - -texture(depthMap, p1.xy).r * heightScale;
    float t = (0.0 - err0) / (err1 - err0);
    currentPos = mix(currentPos - sampleStep, currentPos, t);
    return currentPos;
}

void main() {
    float a = cos(u_time) * 0.5 + 0.5;

    vec3 toEyeDir;
    if (u_isOrthographic) {
        toEyeDir = (inverse(u_V) * vec4(0.0, 0.0, 1.0, 0.0)).xyz;
    } else {
        // toEyeDir = normalize(u_cameraPosW - _in.posW);
        vec3 posV = (u_V * vec4(_in.posW, 1.0)).xyz;
        toEyeDir = (inverse(u_V) * vec4(-posV, 0.0)).xyz;
        toEyeDir = normalize(toEyeDir);
    }

    vec3 toEyeDirT = transpose(_in.tan2world) * toEyeDir;
    vec3 posT = parallaxMap(toEyeDirT, _in.st, u_depthMap, 2, 64);
    vec2 st = posT.xy;
    // st = _in.st;
    // if (st.x < 0.0 || st.x > 1.0 || st.y < 0.0 || st.y > 1.0) { discard; }

    vec3 normalT = texture(u_normalMap, st).rgb * 2.0 - 1.0;
    normalT = normalize(normalT); // Required because of texture interpolation
    // normalT = vec3(0, 0, 1);
    vec3 normalW = _in.tan2world * normalT;

    vec3 diffuseColor = texture(u_diffuseMap, st).rgb;
    // diffuseColor = vec3(0.8, 0.2, 0.05);

    vec3 cSum = vec3(0);
    for (int i = 0; i < u_nLights; i++) {
        vec3 specularColor = u_lights[i].color;
        vec3 lightPos = u_lights[i].pos;
        vec3 toLight = lightPos - _in.posW;
        vec3 toLightDir = normalize(toLight);
        float lightDist = length(toLight);
        float attenuation = 1.0 / (lightDist * lightDist);

        vec3 c = u_lights[i].intensity * attenuation * phong(
            diffuseColor * specularColor,
            specularColor,
            toLightDir,
            normalW,
            toEyeDir,
            0.5
        );
        cSum += c;
    }
    vec3 c = cSum / u_nLights;

    // float shadowDepthCurrent = lightDist / u_shadowmapFarPlane;
    // float shadowDepthClosest = texture(u_shadowmap, -toLight).r;
    // float bias = 0.0;
    // float shadowMod = step(shadowDepthCurrent - bias, shadowDepthClosest);
    // if (shadowDepthClosest == 1.0) { shadowMod = 1.0; }
    // c *= shadowMod;

    c += diffuseColor * 0.01;

    FragColor = vec4(c, 1.0);
    // FragColor = vec4(vec3(_in.st, 0.0), 1.0);
    // FragColor = vec4(vec3(normalW), 1.0);
    // FragColor = vec4(vec3(heightOffsetT), 1.0);
}
