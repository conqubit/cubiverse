#version 330

uniform mat4 worldViewProjectionMat;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

smooth out vec4 fcolor;

void main() {
    gl_Position = worldViewProjectionMat * position;
    fcolor = color;
}