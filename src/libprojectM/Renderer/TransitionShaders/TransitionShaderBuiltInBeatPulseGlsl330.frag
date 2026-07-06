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
}