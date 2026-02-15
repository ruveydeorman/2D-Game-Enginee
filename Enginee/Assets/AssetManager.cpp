#include"AssetManager.h"
#include<iostream>

void AssetManager::Init(SDL_Renderer* r) {
	m_renderer = r;
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}
void AssetManager::Shutdown() {
	for (auto kv : m_Tex) { //unorder mapte dolanýr ve ikinci item olan value yani  texturý destroy eder
		SDL_DestroyTexture(kv.second);
	}
	m_Tex.clear();
	IMG_Quit();
}
SDL_Texture* AssetManager::GetTexture(const std::string& key, const std::string& path) {
	//önbellek kontrolü 
	if (auto a = m_Tex.find(key); a != m_Tex.end()) {
		return a->second;
	}
	const std::string file = path.empty() ? key : path;
	SDL_Surface* surf = IMG_Load(file.c_str());
	if (!surf) {
		std::cerr << "IMG LOAD" << IMG_GetError() << std::endl;
		return nullptr;
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(m_renderer, surf);
	SDL_FreeSurface(surf);
	if (!tex) { std::cerr << "create texture: " << SDL_GetError() << std::endl; return nullptr; }

	m_Tex[key] = tex;
	return tex;
}