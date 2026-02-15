#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_render.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<unordered_map>
#include<string>


class AssetManager {
public:
	void Init(SDL_Renderer* r);
	void Shutdown();
	SDL_Texture* GetTexture(const std::string& key, const std::string& path);
	Mix_Chunk GetSound(const std::string& LogicName);
	void LoadManifest(const std::string& jsonPath);

private:
	SDL_Renderer* m_renderer = nullptr;
	std::unordered_map<std::string, SDL_Texture*>m_Tex;
	std::unordered_map<std::string, Mix_Chunk>m_Snd;
};
