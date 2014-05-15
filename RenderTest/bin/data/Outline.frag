#version 120

varying vec2 texCoord;
varying vec3 position;
varying vec3 color;

void main() {
	float brightness = min(min(color.r, color.g), color.b);
	brightness = brightness < .1 ? 1. : 0.;
    gl_FragColor = vec4(vec3(brightness), 1.);
}