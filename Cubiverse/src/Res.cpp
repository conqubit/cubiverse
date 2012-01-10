#include "stdafx.h"

#include "Res.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"

std::map<string, Shader*> Res::shaders;
std::map<string, Texture*> Res::textures;

bool Res::Init() {
	Shader* shader = nullptr;
	Texture* texture = nullptr;

	// block shader
	shader = new Shader();
	if (!shader->Init("res/block.vert", "res/block.frag")) return false;
	AddShader("block", shader);

	// simpler color shader
	shader = new Shader();
	if (!shader->Init("res/color.vert", "res/color.frag")) return false;
	AddShader("color", shader);


	std::vector<byte> buffer, image;
	LodePNG::loadFile(buffer, "res/blocks.png");
	LodePNG::Decoder decoder;
	decoder.decode(image, buffer);

	int dim = decoder.getWidth();

	texture = Texture::Create3DTexture(dim, dim, decoder.getHeight() / dim, image.data());
	AddTexture("blocks", texture);

	return true;
}

void Res::AddShader(const string& name, Shader* shader) {
	if (shaders.count(name)) {
		shaders[name]->Shutdown();
	}

	shaders[name] = shader;
}

void Res::AddTexture(const string& name, Texture* texture) {
	if (textures.count(name)) {
		textures[name]->Shutdown();
	}

	textures[name] = texture;
}

void Res::Shutdown() {
	for (auto i = shaders.begin(); i != shaders.end(); ++i) {
		i->second->Shutdown();
	}
	for (auto i = textures.begin(); i != textures.end(); ++i) {
		i->second->Shutdown();
	}
	shaders.clear();
	textures.clear();
}