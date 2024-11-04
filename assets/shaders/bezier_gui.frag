#version 460 core

uniform vec2 p0;
uniform vec2 p1;
uniform vec2 p2;
uniform vec2 p3;
uniform uint color0 = 0x000000ff;
uniform uint color1 = 0x000000ff;
uniform float thickness = 5;
uniform uint step_count = 50;

out vec4 color;

vec4 rgba(const uint c) {
	const uint r = c >> 24 & 0xff;
	const uint g = c >> 16 & 0xff;
	const uint b = c >> 8 & 0xff;
	const uint a = c & 0xff;
	return vec4(r, g, b, a) / 255.0;
}

bool inPoint(const vec2 a, const vec2 b) {
	vec2 diff = b - a;
	return dot(diff, diff) <= 0.001;
}

bool inLine(const vec2 p, const vec2 a, const vec2 b, const float d) {
	vec2 line = b - a;
	vec2 diff = p - a;
	float t = clamp(dot(diff, line) / dot(line, line), 0.0, 1.0);
	vec2 c = diff - line * t;
	return dot(c, c) <= d * d;
}

vec2 bezierPoint(const float t) {
	float nt = 1 - t;
	return nt*nt*nt*p0 + 3*nt*nt*t*p1 + 3*nt*t*t*p2 + t*t*t*p3;
}

void main() {
	vec2 pos = gl_FragCoord.xy;
	for (uint i = 0; i < step_count; i++) {
		float t = float(i) / float(step_count);
		vec2 a = bezierPoint(t);
		vec2 b = bezierPoint(float(i + 1) / float(step_count));
		if (inLine(pos, a, b, thickness)) {
			color = mix(rgba(color0), rgba(color1), t);
			return;
		}
	}
//	color = vec4(mix(rgba(color0), rgba(color1), 0.5).rgb, 0.5);
	discard;
}