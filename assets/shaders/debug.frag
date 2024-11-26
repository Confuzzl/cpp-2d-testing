#version 460 core

const vec4 color0 = vec4(0.0, 0.0, 1.0, 1.0);
const vec4 color1 = vec4(0.0, 1.0, 0.0, 1.0);

const vec2 p0 = vec2(50.0, 50.0);
const vec2 p1 = vec2(300.0, 600.0);
const vec2 p2 = vec2(600.0, 500.0);
const vec2 p3 = vec2(800.0, 100.0);

const uint step_count = 10;

out vec4 color;

bool written = false;

void point(const vec2 a, const vec2 b, const float d, const vec3 col) {
	vec2 diff = b - a;
	if (dot(diff, diff) > d * d)
		return;
	color = vec4(col, 1.0);
	written = true;
}

void line(const vec2 p, const vec2 a, const vec2 b, const float d, const vec3 col) {
	vec2 line = b - a;
	vec2 diff = p - a;
	float t = dot(diff, line) / dot(line, line);
	if (t < 0 || t > 1)
		return;
	vec2 projected = a + line * t;
	point(p, projected, d, col);

//	float t = clamp(dot(diff, line) / dot(line, line), 0.0, 1.0);
//	vec2 c = diff - line * t;
//	if (dot(c, c) > d * d)
//		return;
//	color = vec4(col, 1.0);
//	written = true;
}
#define WHITE vec3(1.0, 1.0, 1.0)
#define BLACK vec3(0.0, 0.0, 0.0)
#define GRAY vec3(0.5, 0.5, 0.5)
#define RED vec3(1.0, 0.0, 0.0)
#define GREEN vec3(0.0, 1.0, 0.0)
#define BLUE vec3(0.0, 0.0, 1.0)
#define CYAN vec3(0.0, 1.0, 1.0)
#define MAGENTA vec3(1.0, 0.0, 1.0)
#define YELLOW vec3(1.0, 1.0, 0.0)

const uint lines = 10;
const float lineMin = 1.0;
const float lineMax = 10.0;

void main() {
	
//	line(gl_FragCoord.xy, p1, p3, 10.0, BLUE);
//	point(gl_FragCoord.xy, p1, 10.0, RED);
//	point(gl_FragCoord.xy, p3, 10.0, RED);
//	if(!written)
//		discard;
//	line(gl_FragCoord.xy, p1, p3, 10.0, RED);
//	POINT(p0, 10, BLACK)
//	POINT(p1, 10, BLACK)
//	POINT(p2, 10, BLACK)
//	POINT(p3, 10, BLACK)
//	LINE(p0, p1, 5, GRAY)
//	LINE(p2, p3, 5, GRAY)
//	discard;

//	else
//		discard;
//	if (inPoint(gl_FragCoord.xy, p0, 10) || inPoint(gl_FragCoord.xy, p1, 10) ||
//		inPoint(gl_FragCoord.xy, p2, 10) || inPoint(gl_FragCoord.xy, p3, 10))
//		color = vec4(GRAY, 1.0);
//	else
//		discard;

//	POINT(p0, 10.0, GRAY)
//	POINT(p1, 10.0, GRAY)
//	POINT(p2, 10.0, GRAY)
//	POINT(p3, 10.0, GRAY)

//	if (gl_FragCoord.x > 500.0)
//		color = vec4(0.0, 0.0, 1.0, 1.0);
//	else
//		discard;
//	color = vec4(0.0, 0.0, 1.0, 1.0);
//	if(distance(gl_FragCoord.xy, vec2(500.0, 500.0)) < 50.0)
//		color = vec4(0.0, 0.0, 1.0, 1.0);
//	for (uint i = 0; i < step_count; i++) {
//		float t = float(i) / float(step_count);
//		vec2 a = bezierPoint(t);
//		vec2 b = bezierPoint(float(i + 1) / float(step_count));
//		if (inLine(pos, a, b, thickness)) {
//			color = mix(color0, color1, t);
//			return;
//		}
//	}
//	if (inPoint(pos, vec2(0.0, 0.0))) {
//		color = vec4(0.0, 0.0, 0.0, 1.0);
//	}
}