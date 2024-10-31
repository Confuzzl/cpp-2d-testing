#version 460 core

uniform uint width = 1;
uniform uint spacing = 0;

#define FORWARD 1
#define BACKWARD 2
#define CROSS 3
uniform uint pattern = FORWARD;

uniform uint frag_color = 0xff;

out vec4 color;

vec4 rgba() {
	const uint r = frag_color >> 24 & 0xff;
	const uint g = frag_color >> 16 & 0xff;
	const uint b = frag_color >> 8 & 0xff;
	const uint a = frag_color & 0xff;
	return vec4(r, g, b, a) / 255.0;
}

float pos_mod(const int n, const uint m) {
	return mod(mod(n, m) + m, m);
}

bool check(const float n) {
	return pos_mod(int(n), 2 + spacing) < width;
}

bool forward() {
	return check(gl_FragCoord.x + gl_FragCoord.y);
}
bool backward() {
	return check(gl_FragCoord.x - gl_FragCoord.y);
}

void main() {
	if ((bool(pattern & FORWARD) && forward()) || 
		(bool(pattern & BACKWARD) && backward())) {
		color = rgba();
		return;
	}
	discard;
}