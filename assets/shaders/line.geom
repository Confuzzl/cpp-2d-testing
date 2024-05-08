#version 460 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float thickness = 1;

void main() {
	int ab = int(gl_in[0].gl_Position.x < gl_in[1].gl_Position.x);
	const vec2 left = gl_in[1 - ab].gl_Position.xy, right = gl_in[ab].gl_Position.xy;

	const vec2 perp = normalize(right.yx - left.yx) * vec2(+1.0, -1.0) * thickness;
	// 1---3
	// | \ |
	// 0---2
	vec2 coordinates[4];
	coordinates[0] = left + perp;
	coordinates[1] = left - perp;
	coordinates[2] = right + perp;
	coordinates[3] = right - perp;

	for (int i = 0; i < 4; i++) {
		gl_Position = vec4(coordinates[i], 0, 0);
		EmitVertex();
	}
}