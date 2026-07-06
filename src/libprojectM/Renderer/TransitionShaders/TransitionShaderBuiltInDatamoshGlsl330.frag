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
}