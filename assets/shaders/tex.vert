#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 vertex_uv_in;

layout(std140) uniform ViewBlock {
	mat4 view;
};

out vec2 vertex_uv_out;

<<<<<<< HEAD
=======
layout(std140) uniform ViewBlock {
	mat4 view;
};
>>>>>>> 897504e5eb9f49af32b14b9c1c6d0ff23dc2d678

const int TEXEL_RANGE = (1 << 15) - 1;

void main() {
	gl_Position = view * vec4(pos, 0.0, 1.0);
	vertex_uv_out = vertex_uv_in / TEXEL_RANGE;
}