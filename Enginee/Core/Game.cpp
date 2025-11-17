#include"Game.h"
#include<iostream>

Game::Game() {};
Game::~Game() { Shutdown(); }

bool Game::Init(const std::string& title, int widht, int height) {
	m_windowWidht = widht;
	m_windowHeight = height;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDLinit failed." << SDL_GetError() << std::endl;
		return false;
	}
	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidht, m_windowHeight, SDL_WINDOW_SHOWN);
	

	if (!m_window) {
		std::cerr << "window creating failed." << std::endl;
	}
	
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	
	if (!m_renderer) {
		std::cerr << "renderer creating failed." << std::endl;
	}

	m_isRunning = true;
	return true;

}
//game init ending  and process input start

void Game::ProcessInput() { //create event and controlling any error 
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			m_isRunning = false;
		}
	}
}
void Game::Update(float) {
	//update game loic here
}
void Game::Render() {
	SDL_SetRenderDrawColor(m_renderer, 0, 30, 30, 250);//back ground
	SDL_RenderClear(m_renderer);
	SDL_RenderPresent(m_renderer);
}
void Game::Shutdown() {
	if (m_renderer)SDL_DestroyRenderer(m_renderer);
	if (m_window)SDL_DestroyWindow(m_window);
	SDL_Quit();
}
void Game::Run(){
	while (m_isRunning) {
		ProcessInput();
		Update(0.0f);
		Render();
	}

}

