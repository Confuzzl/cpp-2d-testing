#version 460 core

in vec2 vertex_uv_out;

uniform sampler2D sampler;
uniform uint thickness;
uniform uint direction;
uniform uint frag_color;

out vec4 color;

vec4 rgba() {
	const uint r = frag_color >> 24 & 0xff;
	const uint g = frag_color >> 16 & 0xff;
	const uint b = frag_color >> 8 & 0xff;
	const uint a = frag_color & 0xff;
	return vec4(r, g, b, a) / 255.0;
}

void main() {
	if (texture(sampler, vertex_uv_out).a > 0.0) {
		color = rgba();
		return;
	}

	const uint other_direction = 1 - direction;
	const float uv_step = 1.0 / textureSize(sampler, 0)[direction];

	for (int ln = -int(thickness); ln <= int(thickness); ln++) {
		if (texture(sampler, 
			vertex_uv_out + vec2(ln * uv_step) * vec2(other_direction, direction)
		).a > 0.0) {
			color = rgba();
			return;
		}
	}

	discard;
}