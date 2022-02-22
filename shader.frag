#version 330 core
out vec4 FragColor;

in SHARED {
    vec3 posW;
    vec3 normalW;
    // vec2 st;
    vec4 shadowmapFragCoord;
} _in;

uniform vec3 u_cameraPosW;

uniform float u_time;
uniform sampler2D u_tex0;
uniform sampler2D u_tex1;

uniform struct {
    vec3 pos;
    vec3 color;
    float intensity;
} u_light;

uniform mat4 u_V;
uniform bool u_isOrthographic;

uniform sampler2D u_shadowmap;

##include lighting.glsl

void main() {
    float a = cos(u_time) * 0.5 + 0.5;

    vec3 diffuseColor = texture(u_tex0, _in.posW.xy).rgb;
    // vec3 diffuseColor = vec3(0.8, 0.2, 0.05);
    vec3 specularColor = u_light.color;
    vec3 lightPos = u_light.pos;
    vec3 toLight = lightPos - _in.posW;
    vec3 toEye;
    if (u_isOrthographic) {
        toEye = (inverse(u_V) * vec4(0.0, -1.0, 0.0, 0.0)).xyz;
    } else {
        toEye = u_cameraPosW - _in.posW;
    }
    float lightDist = length(toLight);
    float attenuation = 1.0 / (lightDist * lightDist);

    vec3 c = u_light.intensity * attenuation * phong(
        diffuseColor,
        specularColor,
        toLight,
        _in.normalW,
        toEye,
        0.5
    );

    vec3 shPos = _in.shadowmapFragCoord.xyz / _in.shadowmapFragCoord.w;
    shPos = shPos * 0.5 + 0.5;
    float shadowDepthCurrent = shPos.z;
    float shadowDepthClosest = texture(u_shadowmap, shPos.xy).r;
    float bias = 0.0;
    float shadowMod = step(shadowDepthCurrent - bias, shadowDepthClosest);
    if (shadowDepthClosest == 1.0) { shadowMod = 1.0; }

    c *= shadowMod;
    c += diffuseColor * 0.01;

    // float s = pow(shadowDepthCurrent, 1000.0);
    // float s = pow(shadowDepthClosest, 1000.0);
    // float s = shadowMod;
    // if (s == 0.0) { discard; }

    FragColor = vec4(c, 1.0);
    // FragColor += (1.0 - s);
    // FragColor = vec4(vec3(s), 1.0);
}
