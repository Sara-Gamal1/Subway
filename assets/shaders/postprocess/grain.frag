
#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

in vec2 tex_coord;

out vec4 frag_color;


#define grainIntensity 0.1    // Intensity of the grain effect
   

float rand(vec2 co){
    return fract(cos(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    // Sample final color texture
    vec4 color = texture(tex, tex_coord);

    // Generate random noise
    float noiseValue = rand(gl_FragCoord.xy);

    // Scale noise to match the intensity
    noiseValue = noiseValue * 2.0 - 1.0; // Map noise from [0, 1] to [-1, 1]
    noiseValue *= grainIntensity;

    // Blend noise with final color
    vec3 noisyColor = color.rgb + noiseValue;

    frag_color = vec4(noisyColor, 1);
}
