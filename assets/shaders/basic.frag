#version 460 core

uniform uvec3 frag_color = uvec3(255, 255, 255);

out vec4 color;

void main() {
	color = vec4(frag_color.xyz / 255.0, 1.0);
}