#version 460 core

in vec2 vertex_uv_out;

out vec4 frag_color;

uniform sampler2D sampler;
uniform uvec3 color = uvec3(255, 255, 255);

void main() {
	const vec4 col = texture(sampler, vertex_uv_out) * vec4(color / 255.0, 1.0);
	if (col.a == 0)
		discard;
	frag_color = col;
}