#version 460 core

in vec2 vertex_uv_out;

uniform sampler2D sampler;
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
	const vec4 col = texture(sampler, vertex_uv_out) * rgba();
	if (col.a == 0)
		discard;
	color = col;
}