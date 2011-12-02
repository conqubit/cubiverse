#version 330

uniform mat4 worldViewProjectionMat;

in vec4 position;
in vec4 color;

smooth out vec4 fcolor;

void main() {
    gl_Position = worldViewProjectionMat * position;
    fcolor = color;
}