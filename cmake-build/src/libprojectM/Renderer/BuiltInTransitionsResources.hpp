#pragma once

#include <string>

static std::string kTransitionShaderBuiltInCircleGlsl330 = R"(
// Circular blend with soft edge, inside-out or outside-in
// Source: https://www.shadertoy.com/view/NdGfzG
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord / iResolution.xy;
    float aspect = iResolution.y / iResolution.x;

    // Randomize direction and edge width
    float inOrOut = mod(float(iRandStatic.x) * .01, 2.);

    float progress;
    vec3 imgInner, imgOuter;
    if (inOrOut < 1.)
    {
        imgInner = texture(iChannel0, uv).xyz;
        imgOuter = texture(iChannel1, uv).xyz;
        progress = iProgressCosine;
    }
    else
    {
        imgOuter = texture(iChannel0, uv).xyz;
        imgInner = texture(iChannel1, uv).xyz;
        progress = 1. - iProgressCosine;
    }
    float blendWidth = mod(float(iRandStatic.y) * .001, .1) + .05;
    progress = progress * (1. + blendWidth) - blendWidth;

    // Blending
    vec2 center = vec2(.5);
    float rad = sqrt(center.x / aspect * center.x / aspect + center.y * center.y) * progress;
    float rad2 = rad + blendWidth;
    float r1 = sqrt((uv.x - center.x) / aspect * (uv.x - center.x) / aspect + (uv.y - center.y) * (uv.y - center.y));

    vec3 col;
    if (r1 > rad2)
    {
        col = imgInner;
    }
    else if (r1 > rad)
    {
        float v1=(r1-rad)/(rad2-rad);
        float v2 = 1.0 - v1;
        col = v1 * imgInner + v2 * imgOuter;
    }
    else
    {
        col = imgOuter;
    }

    // Output to screen
    fragColor = vec4(col, 1.0);
}
)";

static std::string kTransitionShaderBuiltInPlasmaGlsl330 = R"(
// Fractal, plasma-like transition effect with random scales and noise seeds.
// Source: https://www.shadertoy.com/view/MstBzf

float sinNoise(vec2 uv)
{
    return fract(abs(sin(uv.x * 0.018 + uv.y * 0.3077) * (float(iRandStatic.x) * .001)));
}

float valueNoise(vec2 uv, float scale)
{
    vec2 luv = fract(uv * scale);
    vec2 luvs = smoothstep(0.0, 1.0, fract(uv * scale));
    vec2 id = floor(uv * scale);
    float tl = sinNoise(id + vec2(0.0, 1.0));
    float tr = sinNoise(id + vec2(1.0, 1.0));
    float t = mix(tl, tr, luvs.x);

    float bl = sinNoise(id + vec2(0.0, 0.0));
    float br = sinNoise(id + vec2(1.0, 0.0));
    float b = mix(bl, br, luvs.x);

    return mix(b, t, luvs.y) * 2.0 - 1.0;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;

    vec3 imgOld = texture(iChannel0, uv).xyz;
    vec3 imgNew = texture(iChannel1, uv).xyz;

    uv.y /= iResolution.x/iResolution.y;

    float sinN = sinNoise(uv);

    float scale = mod(float(iRandStatic.y) * .01, 7.) * 4. + 4.;

    float fractValue = 0.;
    float amp = 1.;
    for(int i = 0; i < 16; i++)
    {
        fractValue += valueNoise(uv, float(i + 1) * scale) * amp;
        amp *= .5;
    }

    fractValue *= .25;
    fractValue += .5;

    float cutoff = smoothstep(iProgressCosine + .1, iProgressCosine - .1, fractValue);
    vec3 col = mix(imgOld, imgNew, cutoff);

    // Output to screen
    fragColor = vec4(col, 1.);
})";

static std::string kTransitionShaderBuiltInSimpleBlendGlsl330 = R"(
// Simple crossfade effect from old to new
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord / iResolution.xy;

    // Blending
    fragColor = vec4(mix(texture(iChannel0, uv).xyz,
                         texture(iChannel1, uv).xyz,
                         iProgressCosine), 1.0);
}
)";

static std::string kTransitionShaderBuiltInSweepGlsl330 = R"(
// Side-to-side sweep with random angle and transition zone width

float atan_y_over_x(float y, float x)
{
    if (x > 0.0) return atan(y / x);
    else if (x < 0.0 && y >= 0.0) return atan(y / x) + 3.14159265;
    else if (x < 0.0 && y < 0.0) return atan(y / x) - 3.14159265;
    else return 1.57079632 * sign(y);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord / iResolution.xy;
    vec3 imgOld = texture(iChannel0, uv).xyz;
    vec3 imgNew = texture(iChannel1, uv).xyz;

    // Blending
    float currentAngle = mod(float(iRandStatic.x), 360.);
    float blendWidth = mod(float(iRandStatic.y) * .001, .3) + .1;

    uv -= .5;
    float angle = radians(90.0) - radians(currentAngle) + atan_y_over_x(uv.y, uv.x);

    float len = length(uv) / sqrt(2.);
    uv = vec2(cos(angle) * len, sin(angle) * len) + .5;

    vec3 col = mix(imgNew, imgOld, smoothstep(iProgressCosine, iProgressCosine + blendWidth, (uv.x / (1.0 + 2. * blendWidth)) + blendWidth));

    // Output to screen
    fragColor = vec4(col, 1.0);
}
)";

static std::string kTransitionShaderBuiltInWarpGlsl330 = R"(
// Horizontal/vertical warp effect
// Source: https://www.shadertoy.com/view/ssj3Dh
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord / iResolution.xy;
    vec3 imgOld = texture(iChannel0, uv).xyz;
    vec3 imgNew = texture(iChannel1, uv).xyz;

    // Randomize direction
    float direction = mod(float(iRandStatic.x) * .01, 4.);
    float coord;
    if (direction < 1.) coord = uv.x;
    else if (direction < 2.) coord = 1. - uv.x;
    else if (direction < 3.) coord = uv.y;
    else coord = 1. - uv.y;

    // Blending
    float x = smoothstep(.0,1.0,(iProgressCosine * 2.0 + coord - 1.0));
    fragColor = mix(texture(iChannel0, (uv - .5) * (1. - x) + .5), texture(iChannel1, (uv - .5) * x + .5), x);
}
)";

static std::string kTransitionShaderBuiltInZoomBlurGlsl330 = R"(
const float strength = 0.3;
const float PI = 3.141592653589793;

float Linear_ease(in float begin, in float change, in float duration, in float time) {
    return change * time / duration + begin;
}

float Exponential_easeInOut(in float begin, in float change, in float duration, in float time) {
    if (time == 0.0)
    return begin;
    else if (time == duration)
    return begin + change;
    time = time / (duration / 2.0);
    if (time < 1.0)
    return change / 2.0 * pow(2.0, 10.0 * (time - 1.0)) + begin;
    return change / 2.0 * (-pow(2.0, -10.0 * (time - 1.0)) + 2.0) + begin;
}

float Sinusoidal_easeInOut(in float begin, in float change, in float duration, in float time) {
    return -change / 2.0 * (cos(PI * time / duration) - 1.0) + begin;
}

float random(in vec3 scale, in float seed) {
    return fract(sin(dot(gl_FragCoord.xyz + seed, scale)) * 43758.5453 + seed);
}

vec3 crossFade(in vec2 uv, in float dissolve) {
    return mix(texture(iChannel0, uv).rgb, texture(iChannel1, uv).rgb, dissolve);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    vec2 texCoord = fragCoord.xy / iResolution.xy;
    float progress = iProgressCosine;
    // Linear interpolate center across center half of the image
    vec2 center = vec2(Linear_ease(0.5, 0.0, 1.0, progress),0.5);
    float dissolve = Exponential_easeInOut(0.0, 1.0, 1.0, progress);

    // Mirrored sinusoidal loop. 0->strength then strength->0
    float strength = Sinusoidal_easeInOut(0.0, strength, 0.5, progress);

    vec3 color = vec3(0.0);
    float total = 0.0;
    vec2 toCenter = center - texCoord;

    /* randomize the lookup values to hide the fixed number of samples */
    float offset = random(vec3(12.9898, 78.233, 151.7182), 0.0)*0.5;

    for (float t = 0.0; t <= 20.0; t++) {
        float percent = (t + offset) / 20.0;
        float weight = 1.0 * (percent - percent * percent);
        color += crossFade(texCoord + toCenter * percent * strength, dissolve) * weight;
        total += weight;
    }

    fragColor = vec4(color / total, 1.0);
})";

static std::string kTransitionVertexShaderGlsl330 = R"(
precision mediump float;

layout(location = 0) in vec2 iPosition;

void main() {
    gl_Position = vec4(iPosition, 0.0, 1.0);
}
)";

static std::string kTransitionShaderHeaderGlsl330 = R"(
// Uniforms
uniform vec3 iResolution;
uniform vec4 durationParams;
uniform vec2 timeParams;
uniform float iFrameRate;
uniform int iFrame;
uniform ivec4 iRandStatic;
uniform ivec4 iRandFrame;
uniform vec3 iBeatValues;
uniform vec3 iBeatAttValues;

#define iProgressLinear durationParams.x
#define iProgressCosine durationParams.y
#define iProgressBicubic durationParams.z
#define iTransitionDuration durationParams.w

#define iTime timeParams.x
#define iTimeDelta timeParams.y

#define iBass iBeatValues.x;
#define iMid iBeatValues.y;
#define iTreb iBeatValues.z;

#define iBassAtt iBeatAttValues.x;
#define iMidAtt iBeatAttValues.y;
#define iTrebAtt iBeatAttValues.z;

// Samplers
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

// These are named as in Milkdrop shaders so we can reuse the code.
uniform sampler2D sampler_noise_lq;
uniform sampler2D sampler_pw_noise_lq;
uniform sampler2D sampler_noise_mq;
uniform sampler2D sampler_pw_noise_mq;
uniform sampler2D sampler_noise_hq;
uniform sampler2D sampler_pw_noise_hq;
uniform sampler3D sampler_noisevol_lq;
uniform sampler3D sampler_pw_noisevol_lq;
uniform sampler3D sampler_noisevol_hq;
uniform sampler3D sampler_pw_noisevol_hq;

#define iNoiseLQ sampler_noise_lq;
#define iNoiseLQNearest sampler_pw_noise_lq;
#define iNoiseMQ sampler_noise_mq;
#define iNoiseMQNearest sampler_pw_noise_mq;
#define iNoiseHQ sampler_noise_hq;
#define iNoiseHQNearest sampler_pw_noise_hq;
#define iNoiseVolLQ sampler_noisevol_lq;
#define iNoiseVolLQNearest sampler_pw_noisevol_lq;
#define iNoiseVolHQ sampler_noisevol_hq;
#define iNoiseVolHQNearest sampler_pw_noisevol_hq;

// Shader output
out vec4 _prjm_transition_out;
)";

static std::string kTransitionShaderMainGlsl330 = R"(

void main() {
    _prjm_transition_out = vec4(1.0, 1.0, 1.0, 1.0);

    vec4 _user_out_color = vec4(1.0, 1.0, 1.0, 1.0);

    mainImage(_user_out_color, gl_FragCoord.xy);

    _prjm_transition_out = vec4(_user_out_color.xyz, 1.0);
})";

static std::string kTransitionShaderBuiltInBeatPulseGlsl330 = R"(
// Helper function for procedural Voronoi cell generation
vec2 fractureHash(vec2 p) {
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(p) * 43758.5453) * 2.0 - 1.0;
}

float voronoiCells(vec2 p, float scale, float progress) {
    p *= scale;
    vec2 p_i = floor(p);
    vec2 p_f = fract(p);
    float minDist = 1.0;
    
    for (int j = -1; j <= 1; j++) {
        for (int i = -1; i <= 1; i++) {
            vec2 neighbor = vec2(float(i), float(j));
            vec2 point = fractureHash(p_i + neighbor);
            // Gentle organic movement driven by progress
            point = 0.5 + 0.5 * sin((progress * 6.2831) + 6.2831 * point);
            vec2 diff = neighbor + point - p_f;
            float dist = length(diff);
            minDist = min(minDist, dist);
        }
    }
    return minDist;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord / iResolution.xy;
    
    // Correct aspect ratio cleanly using wrapper resolution
    float aspect = iResolution.x / (iResolution.y + 0.0001);
    vec2 aspectUv = uv;
    aspectUv.x *= aspect;

    // Use smooth progression
    float progress = smoothstep(0.0, 1.0, iProgressCosine);
    float bassImpact = max(0.0, iBeatValues.x - 1.0) * 0.3;

    // Expand bubble cells as the transition progresses
    float cellScale = 3.5 + (4.0 * progress); 
    float cellMask = voronoiCells(aspectUv, cellScale, progress);
    
    // SAFE NORMALIZATION: Prevents divide-by-zero NaN black screens at screen center
    vec2 dir = uv - vec2(0.5, 0.5);
    float len = length(dir);
    vec2 explosionDir = (len > 0.0001) ? (dir / len) : vec2(0.0, 1.0);
    
    // Outward displacement vector
    float pushSpeed = progress * (0.8 + bassImpact * 1.5);
    vec2 warpedUv = uv + (explosionDir * cellMask * pushSpeed * 0.3);

    // Sample outgoing and incoming scenes
    vec3 sceneOut = texture(iChannel0, warpedUv).xyz;
    vec3 sceneIn  = texture(iChannel1, uv).xyz;
    
    // --- KEY FIX: Smooth Alpha Dissolve ---
    // Instead of a hard step(), we calculate a smooth opacity curve for each bubble.
    // As progress increases, the threshold tightens, causing the bubbles to dissolve
    // smoothly from their outer boundaries inward toward their centers.
    float fadeThreshold = 0.8 * (1.0 - progress);
    float bubbleOpacity = smoothstep(fadeThreshold + 0.2, fadeThreshold, cellMask);
    
    // Global fade-out modifier to ensure complete transparency by the end of the transition
    float globalFade = 1.0 - smoothstep(0.6, 0.98, progress);
    float finalAlpha = clamp(bubbleOpacity * globalFade, 0.0, 1.0);

    // Soft glow along the dissolving bubble rims
    // float rimHighlight = smoothstep(fadeThreshold + 0.1, fadeThreshold, cellMask) - finalAlpha;
    // vec3 softCyanGlow = vec3(0.1, 0.7, 0.9);
    // sceneOut += max(0.0, rimHighlight) * softCyanGlow * 0.5;

    // Smooth composite between incoming background and dissolving outgoing bubbles
    vec3 finalColor = mix(sceneIn, sceneOut, finalAlpha);

    fragColor = vec4(finalColor, 1.0);
})";

static std::string kTransitionShaderBuiltInDatamoshGlsl330 = R"(
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord / iResolution.xy;
    
    // Normalized transition progress (0.0 to 1.0)
    float progress = smoothstep(0.0, 1.0, iProgressCosine);
    
    // 1. ORBITAL ANCHOR (Translating Milkdrop cx/cy sin/cos equations)
    // As the crossfade progresses, the zoom anchor orbits in a figure-8 arc off-center.
    // This creates the sweeping, directional drag current across the screen.
    vec2 dragCenter = vec2(
        0.5 + sin(progress * 5.0) * 0.6,
        0.5 + cos(progress * 3.5) * 0.6
    );
    
    vec2 sampleUv = uv;
    vec3 smearedColor = texture(iChannel0, uv).xyz;
    
    // Dynamically increase the length of the drag smear as progress advances
    int maxIterations = int(mix(4.0, 20.0, progress));
    
    // 2. UNROLLED FEEDBACK LOOP (Translating decay=1.0 & zoom=1.009)
    for (int i = 0; i < 20; i++) 
    {
        if (i >= maxIterations) break;
        
        // Calculate vector from the shifting orbital anchor point
        vec2 offset = sampleUv - dragCenter;
        
        // Milkdrop zoom=1.009 equivalent: calculate outward expansion per step.
        // Brighter pixels (smearedColor.x) drag slightly faster to create dynamic streaks.
        float outwardZoom = 1.0 + (0.008 + (smearedColor.x * 0.015)) * (progress * 1.5);
        
        // Milkdrop rot equivalent: subtle rotational twist along the drag path
        float twist = (smearedColor.y - 0.5) * 0.04 * progress;
        float s = sin(twist);
        float c = cos(twist);
        offset = vec2(offset.x * c - offset.y * s, offset.x * s + offset.y * c);
        
        // Divide by outwardZoom to push pixel coordinate sampling outward
        sampleUv = dragCenter + (offset / outwardZoom);
        
        // Clamp strictly inside [0.001, 0.999] to prevent black texture border bleeding
        sampleUv = clamp(sampleUv, 0.001, 0.999);
        
        // Sample the next step along the coordinate trail
        vec3 trailSample = texture(iChannel0, sampleUv).xyz;
        
        // Accumulate highlights along the trail (simulating decay = 1.0 persistence)
        smearedColor = mix(smearedColor, trailSample, 0.65);
    }
    
    // Sample the clean incoming video frame
    vec3 incomingScene = texture(iChannel1, uv).xyz;
    
    // 3. CLEAN REVEAL CUT
    // Let the melting outgoing frame drag aggressively across the screen for 85% of the transition,
    // then rapidly dissolve into the clean incoming frame so the crossfade resolves perfectly.
    float finalCut = smoothstep(0.85, 0.98, progress);
    vec3 finalOutput = mix(smearedColor, incomingScene, finalCut);
    
    fragColor = vec4(finalOutput, 1.0);
})";


