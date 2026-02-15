#pragma once
#include<iostream>
#include<SDL2/SDL.h>
#include<vector>
#include<algorithm>

struct Sprite {
	SDL_Texture* tex = nullptr;
	SDL_Rect dst{ 0,0,0,0 };
	SDL_Rect src{ 0,0,0,0 };
	int zIndex{ 0 };
};

class RenderQueue {
private:
	std::vector<Sprite>m_sprites;
public:
	void Add(const Sprite& s) { m_sprites.emplace_back(s); }
	void Clear() { m_sprites.clear(); }
	void Flush(SDL_Renderer* r) {
		sort(m_sprites.begin(), m_sprites.end(), [](const Sprite& a, const Sprite& b)
			{return a.zIndex > b.zIndex; });

		for (const auto& s : m_sprites) {
			SDL_RenderCopy(r, s.tex, (s.src.w ? &s.src : nullptr), &s.dst); //kare bitti
		}
		m_sprites.clear();
	}


};
