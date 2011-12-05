#version 330
// Enable this for texture2D arrays.
// #extension GL_EXT_gpu_shader4 : enable

uniform sampler3D textureSampler;

smooth in vec4 fcolor;
smooth in vec3 ftexcoord;

out vec4 colorOut;

void main() {
    colorOut = texture(textureSampler, ftexcoord) * fcolor;
}