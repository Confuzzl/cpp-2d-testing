#version 460 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 view;
uniform float thickness = 1;

uniform bool perspective = true;
uniform mat4 ortho;

void main() {
	int ab = int(gl_in[0].gl_Position.x < gl_in[1].gl_Position.x);
	const vec2 left = gl_in[1 - ab].gl_Position.xy, right = gl_in[ab].gl_Position.xy;

	const vec2 offset = normalize(vec2(left.y - right.y, right.x - left.x)) * thickness;


	if (perspective) {		
		// 0---2
		// | / |
		// 1---3
		const vec2 coordinates[4] = {
			left + offset, left - offset,
			right + offset, right - offset
		};
		for (uint i = 0; i < 4; i++) {
			gl_Position = view * vec4(coordinates[i], 0, 1);
			EmitVertex();
		}
	} else {
		const vec4 fixed_offset = ortho * vec4(1 / ortho[0][0], 1 / ortho[1][1] + 1, 0, 0);
//		const float width = 1 / ortho[0][0], height = 1 / ortho[1][1];
//		const vec4 fixed_offset = ortho * vec4(offset + vec2(width, height), 0, 1);
		gl_Position = view * vec4(left, 0, 1) + fixed_offset;
		EmitVertex();
		gl_Position = view * vec4(left, 0, 1) - fixed_offset;
		EmitVertex();
		gl_Position = view * vec4(right, 0, 1) + fixed_offset;
		EmitVertex();
		gl_Position = view * vec4(right, 0, 1) - fixed_offset;
		EmitVertex();
	}
	
	EndPrimitive();
}