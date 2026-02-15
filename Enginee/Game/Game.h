#pragma once
#include<string>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include"../Core/PhysicsSystem.h"
#include"../Core/RenderQueue.h"
#include"../Assets/AssetManager.h"
#include"../Core/Animation.h"
class Game {

public:
	Game();
	~Game();
	bool Init(const std::string& title, int width, int height);
	void Run();
	void Shutdown();

	//physics
	PhysicsSystem m_Physics;
	b2Body* m_BrickBody;

private: //functions

	void ProcessInput();
	void Update(float deltatime);
	void Render();
	
private: //variables 
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	
	
	bool m_isRunning = false;
	int m_windowWidht = 600;
	int m_windowHeight = 600;
	bool m_isFullScreen = false;

	//animation
	AnimatedSprite m_Anim;
	AnimationClip m_Clip;
	AnimationSystem m_AnimSys;

	//vectors
	struct vec2 { float x{ 0.f },y{ 0.f }; };
	SDL_Texture* m_BrickTex = nullptr;
	SDL_Rect m_BrickDst{ 0,0,0,0 };
	vec2 m_BrickPos{ 0.0f,0.0f };
	vec2 m_BrickVel{ 180.f, 90.f };
	//sorting
	RenderQueue m_RQ;
	//asset
	AssetManager m_Assets;
	//movement
	float m_playerSpeed = 250.f;
	vec2 m_InputDir{ 0.0f,0.0f };

};

