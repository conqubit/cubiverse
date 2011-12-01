#version 330

uniform mat4 proj;
uniform mat4 view;
uniform mat4 world;

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 vcolor;

smooth out vec4 fcolor;

void main() {
    gl_Position = proj * (view * (world * vposition));
    fcolor = vcolor;
}