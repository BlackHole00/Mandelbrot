#version 410 core

precision highp float;

out vec4 frag_color;
in vec2 coord;

uniform float max_iterations;
uniform vec2 position;
uniform vec2 scale;
uniform vec4 color;
uniform float resolution_rateo;
uniform float max_value;

vec2 square_imaginary(vec2 number){
    vec2 vec;
    vec.x = number.x * number.x - number.y * number.y;
    vec.y = 2 * number.x * number.y;

	return vec;
}

float pseudo_lenght(vec2 vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

float iterate_mandelbrot(vec2 local_coord){
//  ORIGINAL ALGORITHM
	vec2 z;
    z.x = local_coord.x;
    z.y = local_coord.y;

	for (int i = 1; i < max_iterations; i++) {
		vec2 tmp = square_imaginary(z);
        z.x = tmp.x + local_coord.x;
        z.y = tmp.y + local_coord.y;

		if (pseudo_lenght(z) > max_value) {
            return i / max_iterations;
        }
	}
	return 1.0;
}

void main() {
    vec2 start_coord;
    start_coord.x = (coord.x / scale.x + position.x) * resolution_rateo;
    start_coord.y = (coord.y / scale.y + position.y);

    float res = iterate_mandelbrot(start_coord);

    frag_color = vec4(res * color.r, res * color.g, res * color.b, 1.0f);
}
