#version 410 core

precision highp float;

out vec4 frag_color;
in vec2 coord;

uniform float maxIterations;
uniform vec2 position;
uniform vec2 scale;
uniform vec4 color;
uniform float resolution_rateo;
uniform float maxValue;

vec2 squareImaginary(vec2 number){
    vec2 vec;
    vec.x = number.x * number.x - number.y * number.y;
    vec.y = 2 * number.x * number.y;

	return vec;
}

float pseudoLenght(vec2 vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

float iterateMandelbrot(vec2 localCoord){
//  ORIGINAL ALGORITHM
	vec2 z;
    z.x = localCoord.x;
    z.y = localCoord.y;

	for (int i = 1; i < maxIterations; i++) {
		vec2 tmp = squareImaginary(z);
        z.x = tmp.x + localCoord.x;
        z.y = tmp.y + localCoord.y;

		if (pseudoLenght(z) > maxValue) {
            return i / maxIterations;
        }
	}
	return 1.0;
}

void main() {
    vec2 startCoord;
    startCoord.x = (coord.x / scale.x + position.x) * resolution_rateo;
    startCoord.y = (coord.y / scale.y + position.y);

    float res = iterateMandelbrot(startCoord);

    frag_color = vec4(res * color.r, res * color.g, res * color.b, 1.0f);
}
