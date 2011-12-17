#include "stdafx.h"

#include "Res.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"

std::map<string, Shader*> Res::shaders;
std::map<string, Texture*> Res::textures;

bool Res::Init() {
    Shader* shader = nullptr;
    Texture* texture = nullptr;

    // Block shader.
    shader = new Shader();
    if (!shader->Init("res/block.v.glsl", "res/block.f.glsl")) return false;
    AddShader("block", shader);

    // Simpler color shader.
    shader = new Shader();
    if (!shader->Init("res/color.v.glsl", "res/color.f.glsl")) return false;
    AddShader("color", shader);

    // Block textures.
    sf::Image image;
    if (!image.LoadFromFile("res/blocks.png")) return false;
    int dim = image.GetWidth();
    texture = Texture::Create3DTexture(dim, dim, image.GetHeight() / dim, (byte*)image.GetPixelsPtr());
    AddTexture("blocks", texture);

    return true;
}

void Res::AddShader(const string& name, Shader* shader) {
    if (shaders.count(name)) {
        shaders[name]->Shutdown();
        delete shaders[name];
    }

    shaders[name] = shader;
}

void Res::AddTexture(const string& name, Texture* texture) {
    if (textures.count(name)) {
        textures[name]->Shutdown();
        delete textures[name];
    }

    textures[name] = texture;
}

void Res::Shutdown() {
    for (auto i = shaders.begin(); i != shaders.end(); i++) {
        i->second->Shutdown();
        delete i->second;
    }
    for (auto i = textures.begin(); i != textures.end(); i++) {
        i->second->Shutdown();
        delete i->second;
    }
    shaders.clear();
    textures.clear();
}