#version 120

uniform mat4 worldViewProjectionMat;

attribute vec4 position;
attribute vec4 color;

varying vec4 fcolor;

void main() {
	gl_Position = worldViewProjectionMat * position;
	fcolor = color;
}