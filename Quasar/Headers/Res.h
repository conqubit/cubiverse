#pragma once

class Shader;
class Texture;

class Res {
private:
    static std::map<string, Shader*> shaders;
    static std::map<string, Texture*> textures;
public:
    static bool Init();
    static void Shutdown();

    static void AddShader(const string& name, Shader* shader);
    static void AddTexture(const string& name, Texture* texture);

    static Shader* GetShader(const string& name) {
        return shaders.count(name) ? shaders[name] : nullptr;
    }

    static Texture* GetTexture(const string& name) {
        return textures.count(name) ? textures[name] : nullptr;
    }
};