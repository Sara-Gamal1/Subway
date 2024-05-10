
#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

in vec2 tex_coord;

out vec4 frag_color;


void main() {
    // Sample final color texture
    vec4 color = texture(tex, tex_coord);
    vec4 final_color = vec4(0.01,0.01,0.01,2.0)+color;

    // frag_color = vec4(final_color, 1);
    frag_color = final_color;
}
