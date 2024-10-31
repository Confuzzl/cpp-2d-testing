#version 460 core

in vec2 vertex_uv_out;

layout (binding = 0) uniform sampler2D sampler;
uniform uint frag_color = 0xff;
uniform float threshold = 0.50;
uniform float font_size;
uniform bool anti_alias = true;

out vec4 color;

vec4 rgba() {
	const uint r = frag_color >> 24 & 0xff;
	const uint g = frag_color >> 16 & 0xff;
	const uint b = frag_color >> 8 & 0xff;
	const uint a = frag_color & 0xff;
	return vec4(r, g, b, a) / 255.0;
}

const float AA_FACTOR = 0.02;

void main() {
	const float alpha = texture(sampler, vertex_uv_out).r;
	if (anti_alias) {
		const vec4 c = rgba();
		color = vec4(c.rgb, c.a * smoothstep(threshold, threshold + AA_FACTOR / max(2, font_size), alpha));
	} else {
		if (alpha <= threshold)
			discard;
		color = rgba();
	}
}