#version 460 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 vertex_uv_in;

out vec2 vertex_uv_out;

uniform mat4 view;

const int TEXEL_RANGE = 1 << 15;

vec2 normalizeUV() {
	vec2 copy = vertex_uv_in;
	copy /= vec2(TEXEL_RANGE, TEXEL_RANGE);
	return copy;
}

void main() {
	gl_Position = view * vec4(pos, 0.0, 1.0);
	vertex_uv_out = normalizeUV();
}