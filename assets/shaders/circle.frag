#version 460 core

uniform vec2 center;
uniform float radius;

uniform vec2 screen_dimensions;
uniform mat4 view;

uniform uvec3 frag_color = uvec3(255, 255, 255);

out vec4 color;

vec2 worldPosition() {
	vec2 ndc = (gl_FragCoord.xy / screen_dimensions - 0.5) * 2.0;
	return (inverse(view) * vec4(ndc, 0.0, 1.0)).xy;
}

void main() {
	const vec2 diff = worldPosition() - center;
	const float d2 = diff.x * diff.x + diff.y * diff.y;
	if (d2 > radius * radius)
		discard;
	color = vec4(frag_color.xyz / 255.0, 1.0);
}