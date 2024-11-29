#version 460 core

layout (location = 0) in vec2 pos;

layout(std140) uniform ViewBlock {
	mat4 view;
};

uniform vec2 parent_pos;
uniform float rotation;

void main() {
	const float s = sin(rotation);
	const float c = cos(rotation);
	const vec2 transformed = vec2(pos.x * c - pos.y * s, pos.x * s + pos.y * c) + parent_pos;

	gl_Position = view * vec4(transformed, 0.0, 1.0);
}

