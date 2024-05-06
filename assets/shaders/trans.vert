#version 460 core

layout (location = 0) in vec2 pos;

uniform vec2 parent_pos = vec2(0.0, 0.0);
uniform float rotation = 0.0;
uniform mat4 view;

void main() {
	const float s = sin(rotation);
	const float c = cos(rotation);
	const vec2 transformed = vec2(pos.x * c - pos.y * s, pos.x * s + pos.y * c) + parent_pos;

	gl_Position = view * vec4(transformed, 0.0, 1.0);
}

