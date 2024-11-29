#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 vertex_uv_in;

layout(std140) uniform ViewBlock {
	mat4 view;
};

out vec2 vertex_uv_out;


const int TEXEL_RANGE = (1 << 15) - 1;

void main() {
	gl_Position = view * vec4(pos, 0.0, 1.0);
	vertex_uv_out = vertex_uv_in / TEXEL_RANGE;
}