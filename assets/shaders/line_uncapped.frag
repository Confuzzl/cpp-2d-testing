#version 460 core

uniform vec2 p0;
uniform vec2 p1;
uniform float thickness;
uniform uint frag_color;

vec4 rgba() {
	const uint r = frag_color >> 24 & 0xff;
	const uint g = frag_color >> 16 & 0xff;
	const uint b = frag_color >> 8 & 0xff;
	const uint a = frag_color & 0xff;
	return vec4(r, g, b, a) / 255.0;
}

out vec4 color;

bool line(const vec2 p, const vec2 a, const vec2 b, const float d) {
	vec2 line = b - a;
	vec2 diff = p - a;
	float t = dot(diff, line) / dot(line, line);
	if (t < 0 || t > 1)
		return false;
	vec2 projected = a + line * t;
	vec2 diff2 = p - projected;
	return dot(diff2, diff2) < d * d;
}

void main() {
	if (!line(gl_FragCoord.xy, p0, p1, thickness))
		discard;
	color = rgba();
}