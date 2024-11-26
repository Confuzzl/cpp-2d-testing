#version 460 core

in vec2 vertex_uv_out;

uniform sampler2D sampler;
uniform uint radius;
uniform uint direction;

out vec4 color;

void main() {
	const uint other_direction = 1 - direction;

	const uint size = 2 * radius + 1;

	const ivec2 dim = textureSize(sampler, 0);

	const int uv_n = int(floor((vertex_uv_out * dim)[direction]));
	
	vec4 c = vec4(0.0);
	for (int ln = -int(radius);  ln <= int(radius); ln++) {
		const int off_n = uv_n + ln;
		const uint off_n_norm = uint(off_n < 0 ? -off_n : off_n);
		const float n = float(off_n_norm) / dim[direction];

		c += texture(sampler, vec2(
			direction * vertex_uv_out.x + other_direction * n, 
			other_direction * vertex_uv_out.y + direction * n
		));
	}
	c /= size;
	if (c.a == 0)
		discard;
	color = c;
}