#version 460 core

<<<<<<< HEAD
=======
uniform float radius;
uniform vec2 center;
>>>>>>> 897504e5eb9f49af32b14b9c1c6d0ff23dc2d678
layout(std140) uniform ViewBlock {
	mat4 view;
};
layout(std140) uniform ScreenBlock {
	uvec2 screen_dimensions;
};
<<<<<<< HEAD

uniform float radius;
uniform vec2 center;
=======
>>>>>>> 897504e5eb9f49af32b14b9c1c6d0ff23dc2d678

uniform uint frag_color = 0xff;

out vec4 color;

vec4 rgba() {
	const uint r = frag_color >> 24 & 0xff;
	const uint g = frag_color >> 16 & 0xff;
	const uint b = frag_color >> 8 & 0xff;
	const uint a = frag_color & 0xff;
	return vec4(r, g, b, a) / 255.0;
}

vec2 worldPosition() {
	vec2 ndc = (gl_FragCoord.xy / screen_dimensions - 0.5) * 2.0;
	return (inverse(view) * vec4(ndc, 0.0, 1.0)).xy;
}

void main() {
//	const vec2 diff = worldPosition();
	const vec2 diff = worldPosition() - center;
//	const float edge = smoothstep(radius * radius - 0.5, radius * radius + 0.1, d2);
//	if (edge >= 1)
//		discard;
// color = vec4(frag_color.xyz / 255.0, 1 - edge);

	if (dot(diff, diff) > radius * radius)
		discard;
	color = rgba();
}