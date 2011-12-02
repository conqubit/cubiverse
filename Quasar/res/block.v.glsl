#version 330

uniform mat4 worldViewProjectionMat;

in vec4 position;
in vec4 color;
in vec2 texcoord;

smooth out vec4 fcolor;
smooth out vec2 ftexcoord;

void main() {
    gl_Position = worldViewProjectionMat * position;
    fcolor = color;
    ftexcoord = texcoord;
}