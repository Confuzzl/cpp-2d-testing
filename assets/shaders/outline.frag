#version 460 core

in vec2 vertex_uv_out;

uniform sampler2D sampler;
uniform uint thickness;
uniform uint direction;
uniform uint frag_color;

out vec4 color;

vec4 rgba() {
	const uint r = frag_color >> 24 & 0xff;
	const uint g = frag_color >> 16 & 0xff;
	const uint b = frag_color >> 8 & 0xff;
	const uint a = frag_color & 0xff;
	return vec4(r, g, b, a) / 255.0;
}

void main() {
	if (texture(sampler, vertex_uv_out).a > 0.0) {
		color = rgba();
<<<<<<< HEAD
		return;
	}

	const uint other_direction = 1 - direction;
	const float uv_step = 1.0 / textureSize(sampler, 0)[direction];

	for (int ln = -int(thickness); ln <= int(thickness); ln++) {
		if (texture(sampler, 
			vertex_uv_out + vec2(ln * uv_step) * vec2(other_direction, direction)
		).a > 0.0) {
			color = rgba();
=======
//		color = vec4(1.0, 0.0, 0.0, 1.0);
		return;
	}
	const uint other_direction = 1 - direction;

	const ivec2 dim = textureSize(sampler, 0);

	const int uv_n = int((vertex_uv_out * dim)[direction]);
	
	const float uv_step = 1.0 / dim[direction];

	for (int ln = -int(thickness); ln <= int(thickness); ln++) {
		const int off_n = uv_n + ln;
		const float n = float(off_n) / dim[direction];

		if (texture(sampler, vec2(
			vertex_uv_out.x + ln * uv_step * other_direction,
			vertex_uv_out.y + ln * uv_step * direction
			// x:
			// vertex_uv_out.x + uv_step, vertex_uv_out.y
			// y:
			// vertex_uv_out.x, vertex_uv_out.y + uv_step
//			other_direction * vertex_uv_out.x + uv_step,
//			direction * vertex_uv_out.y
//			direction * vertex_uv_out.x + other_direction * n, 
//			other_direction * vertex_uv_out.y + direction * n
		)).a > 0.0) {
			vec4 c = rgba();
//			c.a = 1.0 - float(ln * ln) / ((thickness + 1) * (thickness + 1));
//			c.a = exp(-float(ln * ln) / thickness);
//			c.a *= 1.0 - smoothstep(0.0, 1.0, abs(ln) / float(thickness));
			color = c;
>>>>>>> 897504e5eb9f49af32b14b9c1c6d0ff23dc2d678
			return;
		}
	}

	discard;
}