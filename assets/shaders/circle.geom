#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout(std140) uniform ViewBlock {
	mat4 view;
};

uniform float radius = 1;

void main() {
	const vec2 center = gl_in[0].gl_Position.xy;
	// 0---2
	// | c |
	// 1---3
	const vec2 coordinates[4] = {
		center + vec2(-radius, +radius), center + vec2(-radius, -radius),
		center + vec2(+radius, +radius), center + vec2(+radius, -radius)
	};
	for (uint i = 0; i < 4; i++) {
		gl_Position = view * vec4(coordinates[i], 0, 1);
		EmitVertex();
	}
	EndPrimitive();
}