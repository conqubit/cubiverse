#version 120

uniform mat4 worldViewProjectionMat;

attribute vec4 position;
attribute vec4 color;
attribute vec3 texcoord;

varying vec4 fcolor;
varying vec3 ftexcoord;

void main() {
	gl_Position = worldViewProjectionMat * position;
	fcolor = color;
	ftexcoord = texcoord;
}