#version 330

uniform sampler2D textureSampler;

smooth in vec4 fcolor;
smooth in vec2 ftexcoord;

out vec4 colorOut;

void main() {
    colorOut = texture2D(textureSampler, ftexcoord) * fcolor;
}