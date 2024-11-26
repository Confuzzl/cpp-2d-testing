#version 460 core

uniform vec2 p0;
uniform vec2 p1;
uniform vec2 p2;
uniform vec2 p3;
uniform uint color0;
uniform uint color1;
uniform float thickness;
uniform uint step_count;
layout(std140) uniform ViewBlock {
	mat4 view;
};
layout(std140) uniform ScreenBlock {
	uvec2 screen_dimensions;
};
uniform bool world;
uniform bool debug;

out vec4 color;

vec4 rgba(const uint c) {
	const uint r = c >> 24 & 0xff;
	const uint g = c >> 16 & 0xff;
	const uint b = c >> 8 & 0xff;
	const uint a = c & 0xff;
	return vec4(r, g, b, a) / 255.0;
}

bool inPoint(const vec2 a, const vec2 b, const float d) {
	vec2 diff = b - a;
	return dot(diff, diff) <= d * d;
}

bool inLine(const vec2 p, const vec2 a, const vec2 b, const float d) {
	vec2 line = b - a;
	vec2 diff = p - a;
	float t = clamp(dot(diff, line) / dot(line, line), 0.0, 1.0);
	vec2 c = diff - line * t;
	return dot(c, c) <= d * d;
}


bool inDottedLine(const vec2 p, const vec2 a, const vec2 b, const float d) {
	vec2 line = b - a;
	vec2 diff = p - a;
	float t = clamp(dot(diff, line) / dot(line, line), 0.0, 1.0);
	if (mod(t * 10.0, 2.0) < 1.0)
		return false;
	vec2 c = diff - line * t;
	return dot(c, c) <= d * d;
}

vec2 bezierPoint(const float t) {
	float nt = 1 - t;
	return nt*nt*nt*p0 + 3*nt*nt*t*p1 + 3*nt*t*t*p2 + t*t*t*p3;
}

vec2 worldPosition() {
	vec2 ndc = (gl_FragCoord.xy / screen_dimensions - 0.5) * 2.0;
	return (inverse(view) * vec4(ndc, 0.0, 1.0)).xy;
}

#define DEBUG_POINT(p) if (inPoint(pos, p, thickness * 2.0)) { color = vec4(0.0, 0.0, 0.0, 1.0); return; }
#define DEBUG_LINE(a, b) if (inLine(pos, a, b, thickness / 2.0)) { color = vec4(0.5, 0.5, 0.5, 1.0); return; }
#define DEBUG_DOTTED_LINE(a, b) if (inDottedLine(pos, a, b, thickness / 3.0)) { color = vec4(0.5, 0.5, 0.5, 1.0); return; }

void main() {
	vec2 pos = world ? worldPosition() : gl_FragCoord.xy;
	for (uint i = 0; i < step_count; i++) {
		float t = float(i) / float(step_count);
		vec2 a = bezierPoint(t);
		vec2 b = bezierPoint(float(i + 1) / float(step_count));
		if (inLine(pos, a, b, thickness)) {
			color = mix(rgba(color0), rgba(color1), t);
			return;
		}
	}
	if (!debug)
		discard;
	DEBUG_POINT(p0)
	DEBUG_POINT(p1)
	DEBUG_POINT(p2)
	DEBUG_POINT(p3)
	DEBUG_LINE(p0, p1)
	DEBUG_LINE(p2, p3)
	DEBUG_DOTTED_LINE(p1, p2);
	color = vec4(mix(rgba(color0), rgba(color1), 0.5).rgb, 0.1);
}