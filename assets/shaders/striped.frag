#version 460 core

uniform vec2 screen_dimensions;
uniform uint spacing = 0;

uniform uvec3 frag_color = uvec3(255, 255, 255);

out vec4 color;

void main() {
	const uint m = 2 + spacing;
	const vec2 screen_coord = gl_FragCoord.xy * screen_dimensions;
	if (mod(screen_coord.x, m) != 0 || mod(screen_coord.y, m) != 0)
		discard;
	color = vec4(frag_color.xyz / 255.0, 1.0);
}