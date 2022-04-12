#line 2
// Failed assertion causes fragment to blink with a random color.
// Fragment blinks `__LINE__ / 10` times. (5 times for assertions on lines 50 - 59)
//
// Put ASSERT_APPLY() at the end of your fragment shader

#define ASSERT_ENABLED

bool assertFailed = false;
int assertLine = 0;

#ifdef ASSERT_ENABLED
#define ASSERT(b) do { if (!(b) && !assertFailed) { assertFailed=true; assertLine=(__LINE__); } } while(false)
#define ASSERT_LEN(vec, len) ASSERT(abs(length((vec)) - (len)) < 0.001)
#define ASSERT_APPLY(out_var) do { if (assertFailed == true) { out_var = getAssertColor(u_time); } } while(false)
#else
#define ASSERT(b)
#define ASSERT_LEN(vec, len)
#define ASSERT_APPLY(out_var)
#endif

float nBlinks(float t, int n) {
    return step(0.5,fract(clamp((fract(((t+1.0)/(n+1.0)))-0.5)/(1.0-1.0/(n+1.0))+0.5,0.0,1.0)*n));
}

vec4 getAssertColor(float time) {
    const float pi = 3.141592653589793;
    uint x = uint(assertLine);
    float t = float((x ^ x>>16) * 0x9c2d0d6fU) / float(0xffffffffU);
    vec3 c = cos((vec3(0,1,2)/3.-t)*pi*2.)*.5+.5;
    c *= nBlinks(time * 2, assertLine / 10);
    return vec4(c, 1.0);
}
