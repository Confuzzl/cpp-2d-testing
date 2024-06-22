#version 460 core

in vec2 vertex_uv_out;

layout (binding = 0) uniform sampler2D sampler;
uniform uvec3 frag_color = uvec3(255, 255, 255);
uniform float threshold = 0.5;
uniform float font_size;
uniform bool anti_alias = true;

out vec4 color;

const float AA_FACTOR = 0.02;

void main() {
	const float alpha = texture(sampler, vertex_uv_out).r;
	if (anti_alias) {
		color = vec4(frag_color, smoothstep(threshold, threshold + AA_FACTOR / max(1, font_size), alpha));
	} else {
		if (alpha <= threshold)
			discard;
		color = vec4(frag_color, 1);
	}
}