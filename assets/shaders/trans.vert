#version 460 core

layout (location = 0) in vec2 pos;

<<<<<<< HEAD
layout(std140) uniform ViewBlock {
	mat4 view;
};

uniform vec2 parent_pos;
uniform float rotation;
=======
uniform vec2 parent_pos;
uniform float rotation;
layout(std140) uniform ViewBlock {
	mat4 view;
};
>>>>>>> 897504e5eb9f49af32b14b9c1c6d0ff23dc2d678

void main() {
	const float s = sin(rotation);
	const float c = cos(rotation);
	const vec2 transformed = vec2(pos.x * c - pos.y * s, pos.x * s + pos.y * c) + parent_pos;

	gl_Position = view * vec4(transformed, 0.0, 1.0);
}

