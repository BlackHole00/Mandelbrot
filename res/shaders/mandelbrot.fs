#version 410 core

#define complexMul(a, b) vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x)

precision highp float;

out vec4 frag_color;
in vec2 coord;

uniform float maxIterations;
uniform vec2 position;
uniform vec2 scale;
uniform vec4 color;
uniform vec2 resolution;
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
//  NEW ALGORITHM: https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set
//    vec2 xy = vec2(0, 0);
//    vec2 xy0 = coord;
//    vec2 xy2 = vec2(0, 0);
//
//    int i = 0;
//    while ((xy2.x + xy2.y) < 4 && i < maxIterations) {
//        xy.y = 2 * xy.x * xy.y + xy0.y;
//        xy.x = (xy2.x - xy2.y) + xy0.x;
//        xy2.x = xy.x * xy.x;
//        xy2.y = xy.y * xy.y;
//
//        i++;
//    }
//
//    return i / maxIterations;

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
    startCoord.x = (coord.x / scale.x + position.x);
    startCoord.y = (coord.y / scale.y + position.y);

    //startCoord.x = ((coord.x + position.x * scale.x) / scale.x);
    //startCoord.y = ((coord.y + position.y * scale.y) / scale.y);

    float res = iterateMandelbrot(startCoord);

    frag_color = vec4(res * color.r, res * color.g, res * color.b, 1.0f);
}
//// high precision float (very slow)
//dvec3 fnor(dvec3 a) {
//    dvec3 c=a;
//    if (abs(c.x)>1e-5){ c.y+=c.x; c.x=0.0; }
//    if (abs(c.y)>1e+5){ c.z+=c.y; c.y=0.0; }
//    return c;
//}
//double fget(dvec3 a) { return a.x+a.y+a.z; }
//dvec3 fset(double a) { return fnor(dvec3(a,0.0,0.0)); }
//dvec3 fadd(dvec3 a,double b) { return fnor(a+fset(b)); }
//dvec3 fsub(dvec3 a,double b) { return fnor(a-fset(b)); }
//dvec3 fmul(dvec3 a,double b) { return fnor(a*b); }
//dvec3 fadd(dvec3 a,dvec3 b) { return fnor(a+b); }
//dvec3 fsub(dvec3 a,dvec3 b) { return fnor(a-b); }
//dvec3 fmul(dvec3 a,dvec3 b) {
//    dvec3 c;
//    c =fnor(a*b.x);
//    c+=fnor(a*b.y);
//    c+=fnor(a*b.z);
//    return fnor(c);
//}
//
//struct Vec2f192 {
//    dvec3 x;
//    dvec3 y;
//};
//
//Vec2f192 squareImaginary(Vec2f192 number){
//    Vec2f192 vec;
//    //  vec.x = number.x * number.x - number.y * number.y;
//    vec.x = fsub(fmul(number.x, number.x), fmul(number.y, number.y));
//    //  vec.y = 2 * number.x * number.y;
//    vec.y = fmul(fmul(number.x, number.y), 2.0);
//
//	return vec;
//}
//
//double lenghtVec2f192(Vec2f192 vec) {
//    //return vec.x * vec.x + vec.y * vec.y;
//    return fget(fadd(fmul(vec.x, vec.x), fmul(vec.y, vec.y)));
//}
//
//float iterateMandelbrot(Vec2f192 coord){
//	Vec2f192 z;
//    z.x = fset(0);
//    z.y = fset(0);
//
//	for (int i = 0; i < maxIterations; i++) {
//		Vec2f192 tmp = squareImaginary(z);
//
//        //z.x = tmp.x + coord.x;
//        //z.y = tmp.y + coord.y;
//        z.x = fadd(tmp.x, coord.x);
//        z.y = fadd(tmp.y, coord.y);
//
//		if (lenghtVec2f192(z) > 4.0) {
//            return i / maxIterations;
//        }
//	}
//	return maxIterations;
//}
//
//void main() {
//    Vec2f192 startCoord;
//    startCoord.x = fset(double(coord.x) / double(scale.x) + double(position.x));
//    startCoord.y = fset(double(coord.y) / double(scale.y) + double(position.y));
//
//    float res = iterateMandelbrot(startCoord);
//
//    frag_color = vec4(res * color.r, res * color.g, res * color.b, 1.0f);
//}

//struct Vec2d // user defined structure.
//{
//    double x;
//    double y;
//};
//
//Vec2d squareImaginary(Vec2d number){
//    Vec2d vec;
//    vec.x = number.x * number.x - number.y * number.y;
//    vec.y = 2 * number.x * number.y;
//
//	return vec;
//}
//
//double lenghtVec2d(Vec2d vec) {
//    return sqrt(vec.x * vec.x + vec.y * vec.y);
//}
//
//double iterateMandelbrot(Vec2d coord){
//	Vec2d z;
//    z.x = 0;
//    z.y = 0;
//
//	for (int i = 0; i < maxIterations; i++) {
//		Vec2d tmp = squareImaginary(z);
//        z.x = tmp.x + coord.x;
//        z.y = tmp.y + coord.y;
//
//		if (lenghtVec2d(z) > 2) {
//            return i / double(maxIterations);
//        }
//	}
//	return double(maxIterations);
//}
//
//void main() {
//    Vec2d startCoord;
//    startCoord.x = coord.x / scale.x + position.x;
//    startCoord.y = coord.y / scale.y + position.y;
//
//    double res = iterateMandelbrot(startCoord);
//
//    frag_color = vec4(res * color.r, res * color.g, res * color.b, 1.0f);
//}

//void main()
//{
//    float   real  = coord.x / scale.x + position.x;
//    float   imag  = coord.y / scale.y + position.y;
//    float   Creal = real;
//    float   Cimag = imag;
//
//    float r2 = 0.0;
//
//    for (float iter = 0.0; iter < MaxIterations && r2 < 4.0; ++iter)
//    {
//        float tempreal = real;
//
//        real = (tempreal * tempreal) - (imag * imag) + Creal;
//        imag = 2.0 * tempreal * imag + Cimag;
//        r2   = (real * real) + (imag * imag);
//    }
//
//    vec3 color;
//
//    if (r2 < 4.0)
//        color = vec3(0.0f, 0.0f, 0.0f);
//    else
//        color = vec3(1.0f, 1.0f, 1.0f);
//
//    frag_color = vec4(color, 1.0);
//}