#version 460 core

in vec2 vertex_uv_out;

uniform sampler2D sampler;
uniform uvec3 frag_color = uvec3(255, 255, 255);

out vec4 color;

void main() {
	const vec4 col = texture(sampler, vertex_uv_out) * vec4(frag_color / 255.0, 1.0);
	if (col.a == 0)
		discard;
	color = col;
}