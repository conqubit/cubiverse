#version 330

uniform mat4 worldViewProjectionMat;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 texcoord;

smooth out vec4 fcolor;
smooth out vec3 ftexcoord;

void main() {
    gl_Position = worldViewProjectionMat * position;
    fcolor = color;
    ftexcoord = texcoord;
}