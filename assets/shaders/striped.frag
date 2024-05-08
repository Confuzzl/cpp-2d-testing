#version 460 core

uniform uint width = 10;
uniform uint spacing = 0;

#define FORWARD 1
#define BACKWARD 2
#define CROSS 3
uniform uint pattern = FORWARD;

uniform uvec3 frag_color = uvec3(255, 255, 255);

out vec4 color;




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
	if (bool(pattern & FORWARD) && forward()) {
		color = vec4(frag_color.xyz / 255.0, 1.0);
		return;
	}
	if (bool(pattern & BACKWARD) && backward()) {
		color = vec4(frag_color.xyz / 255.0, 1.0);
		return;
	}
	discard;
}