#version 120

uniform sampler3D textureSampler;

varying vec4 fcolor;
varying vec3 ftexcoord;

void main() {
    gl_FragColor = texture3D(textureSampler, ftexcoord) * fcolor;
}