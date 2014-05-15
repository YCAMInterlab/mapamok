#version 120

varying vec2 texCoord;
varying vec3 position;
varying vec3 color;

void main() {
    texCoord = gl_MultiTexCoord0.xy;
    position = gl_Vertex.xyz;
    color = gl_Color.rgb;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}