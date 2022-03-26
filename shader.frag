#version 330 core
out vec4 FragColor;

in SHARED {
    vec3 posW;
    vec2 st;
    mat3 tan2world;
    vec3 posM;
} _in;

uniform vec3 u_cameraPosW;

uniform float u_time;
uniform sampler2D u_tex1;
uniform sampler2D u_tex2;
uniform sampler2D u_tex3;

uniform struct {
    vec3 pos;
    vec3 color;
    float intensity;
} u_light;

uniform mat4 u_V;
uniform bool u_isOrthographic;

uniform float u_shadowmapFarPlane;
uniform samplerCube u_shadowmap;

##include lighting.glsl

void main() {
    float a = cos(u_time) * 0.5 + 0.5;

    vec3 toEye;
    if (u_isOrthographic) {
        toEye = (inverse(u_V) * vec4(0.0, 0.0, 1.0, 0.0)).xyz;
    } else {
        toEye = u_cameraPosW - _in.posW;
    }


    vec3 toEyeDirT = normalize(inverse(_in.tan2world) * toEye);

    const float heightScale = 0.15;
    const float numLayers = 8.0;
    vec3 sampleStep = -toEyeDirT.xyz / toEyeDirT.z * heightScale / numLayers;

    vec3 currentPos = vec3(_in.st, 0.0);

    int i = 0;
    while(currentPos.z > -texture(u_tex3, currentPos.xy).r * heightScale) {
        if (++i > 100) { FragColor=vec4(1,0,1,1); return; break; }
        currentPos += sampleStep;
    }

    vec2 st = currentPos.xy;
    // st = _in.st;
    // if (st.x < 0.0 || st.x > 1.0 || st.y < 0.0 || st.y > 1.0) { discard; }


    vec3 diffuseColor = texture(u_tex1, st).rgb;
    // diffuseColor = vec3(0.8, 0.2, 0.05);
    vec3 specularColor = u_light.color;
    vec3 lightPos = u_light.pos;
    vec3 toLight = lightPos - _in.posW;
    float lightDist = length(toLight);
    float attenuation = 1.0 / (lightDist * lightDist);

    vec3 normalT = texture(u_tex2, st).rgb * 2.0 - 1.0;
    // normalT = vec3(0, 0, 1);
    vec3 normalW = _in.tan2world * normalT;

    vec3 c = u_light.intensity * attenuation * phong(
        diffuseColor,
        specularColor,
        toLight,
        normalW,
        toEye,
        0.5
    );

    float shadowDepthCurrent = lightDist / u_shadowmapFarPlane;
    float shadowDepthClosest = texture(u_shadowmap, -toLight).r;
    float bias = 0.0;
    float shadowMod = step(shadowDepthCurrent - bias, shadowDepthClosest);
    if (shadowDepthClosest == 1.0) { shadowMod = 1.0; }
    c *= shadowMod;

    c += diffuseColor * 0.01;

    FragColor = vec4(c, 1.0);
    // FragColor = vec4(vec3(_in.st, 0.0), 1.0);
    // FragColor = vec4(vec3(normalW * 0.5 + 0.5), 1.0);
    // FragColor = vec4(vec3(heightOffsetT), 1.0);
}
