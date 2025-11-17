#pragma once
#include<string>
#include<SDL2/SDL.h>

class Game {

public:
	Game();
	~Game();
	bool Init(const std::string& title, int width, int height);
	void Run();
	void Shutdown();

private: //functions

	void ProcessInput();
	void Update(float deltatime);
	void Render();

private: //variables 
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	bool m_isRunning = false;
	int m_windowWidht = 800;
	int m_windowHeight = 600;

};
