#version 330
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DArray textureSampler;

smooth in vec4 fcolor;
smooth in vec3 ftexcoord;

out vec4 colorOut;

void main() {
    colorOut = texture2DArray(textureSampler, ftexcoord) * fcolor;
}