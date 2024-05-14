#version 460 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 view;
uniform float thickness = 1;

void main() {
	int ab = int(gl_in[0].gl_Position.x < gl_in[1].gl_Position.x);
	const vec2 left = gl_in[1 - ab].gl_Position.xy, right = gl_in[ab].gl_Position.xy;

	// 0---2
	// | / |
	// 1---3

	const vec2 offset = normalize(vec2(left.y - right.y, right.x - left.x)) * thickness;
	const vec2 coordinates[4] = {
		left + offset, left - offset,
		right + offset, right - offset
	};
	for (uint i = 0; i < 4; i++) {
		gl_Position = view * vec4(coordinates[i], 0.0, 1.0);
		EmitVertex();
	}
	EndPrimitive();
}