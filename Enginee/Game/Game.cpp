#include"Game.h"
#include<iostream>
#include<SDL2/SDL_image.h>

constexpr int TARGET_FPS = 60;
constexpr int FRAME_DELAY = 1000 / TARGET_FPS;
constexpr float PPM = 100.F;
Game::Game() {};
Game::~Game() { Shutdown(); }

bool Game::Init(const std::string& title, int widht, int height) {
	m_windowWidht = widht;
	m_windowHeight = height;
	m_Physics.Init(PPM, 0.f, 0.f);
	//init video
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDLinit failed." << SDL_GetError() << std::endl;
		return false;
	}
	//window
	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidht, m_windowHeight, SDL_WINDOW_SHOWN);


	if (!m_window) {
		std::cerr << "window creating failed." << std::endl;
	}
	//renderer
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!m_renderer) {
		std::cerr << "renderer creating failed." << std::endl;
	}
	//texture
	m_Assets.Init(m_renderer);
	m_BrickTex = m_Assets.GetTexture("brick", "Assets/Texture/Adsiz.png");
	if (!m_BrickTex)return false;
	SDL_QueryTexture(m_BrickTex, nullptr, nullptr, &m_BrickDst.w, &m_BrickDst.h);
	//animation clip
	SDL_Texture* firetex = m_Assets.GetTexture("fire_look", "Assets/Texture/kus.jpg");
	const int w = 64, h = 64, frames =6;
	for (int i = 0; i < frames; i++) {
		m_Clip.frames.push_back({i*w,0,w,h });
	}
	m_Clip.tex = firetex;
	m_Clip.fps = 8.0f;
    
	m_Anim.SetClip(&m_Clip);
	m_Anim.x = 400;
	m_Anim.y = 200;
	m_AnimSys.Add(&m_Anim);

	//BR CK PHS CS
	m_BrickBody = m_Physics.CreateBox(m_BrickPos.x + m_BrickDst.w / 2, m_BrickPos.y + m_BrickDst.h / 2, (float)m_BrickDst.w, (float)m_BrickDst.h, true, 0.8f);
	m_Physics.CreateBox(m_windowWidht / 2, 0, m_windowWidht, 10, false);
	m_Physics.CreateBox(m_windowWidht / 2, m_windowHeight, m_windowWidht, 10, false);
	m_Physics.CreateBox(0, m_windowHeight / 2, 10, m_windowHeight, false);
	m_Physics.CreateBox(m_windowWidht, m_windowHeight / 2, 10, m_windowHeight, false);
	m_isRunning = true;
	return true;



}
//game init ending  and process input start

void Game::ProcessInput() { //create event and controlling any error 
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			std::cout << "[EVENT] Quit event." << std::endl;
			m_isRunning = false;
			break;
		case SDL_KEYDOWN:
			std::cout << "[EVENT] Key pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				m_isRunning = false;
			}
			if (event.key.keysym.sym == SDLK_F11) {
				m_isFullScreen = !m_isFullScreen;
				if (m_isFullScreen) {
					SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
				else {
					SDL_SetWindowFullscreen(m_window, 0);
				}
			}
			break;
		case SDL_KEYUP:
			std::cout << "[EVENT] Key relased: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
			break;
		case SDL_MOUSEMOTION:
			std::cout << "[EVENT] Mouse move:" << event.motion.x << "," << event.motion.y << std::endl;
			break;
		case SDL_MOUSEBUTTONDOWN:
			std::cout << "[EVENT] Mouse button pressed: " << (int)event.button.button << "X=" << event.button.x << "Y=" << event.button.y << std::endl;
			break;
		default:
			break;
		}
		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		m_InputDir = { 0.0f,0.0f };
		if (keys[SDL_SCANCODE_W])m_InputDir.y = -1.f;
		if (keys[SDL_SCANCODE_S])m_InputDir.y = 1.f;
		if (keys[SDL_SCANCODE_A])m_InputDir.x = -1.f;
		if (keys[SDL_SCANCODE_D])m_InputDir.x = 1.f;



	}
}
void Game::Update(float dt) {
	//konum g ncelleme
	//WASD hareketi
	m_BrickPos.x += m_InputDir.x * m_playerSpeed * dt;
	m_BrickPos.y += m_InputDir.y * m_playerSpeed * dt;
	if (m_BrickPos.x < 0)m_BrickPos.x = 0;
	if (m_BrickPos.x + m_BrickDst.w > m_windowWidht) {
		m_BrickPos.x = m_windowWidht - m_BrickDst.w;
	}
	if (m_BrickPos.y < 0)m_BrickPos.y = 0;
	if (m_BrickPos.y + m_BrickDst.h > m_windowHeight) {
		m_BrickPos.y = m_windowHeight - m_BrickDst.h;
	}
	m_Physics.Step(dt);
	//wasd momentum fizi i
	b2Vec2 impulse{ m_InputDir.x * 0.1f,m_InputDir.y * 0.1f };
	if (impulse.LengthSquared() > 0) {
		m_BrickBody->ApplyLinearImpulseToCenter(impulse, true);
	}
	b2Vec2 p = m_BrickBody->GetPosition();

	m_BrickPos.x = p.x * PPM - m_BrickDst.w / 2;
	m_BrickPos.y = p.y * PPM - m_BrickDst.h / 2;
   //animation
	m_AnimSys.Update(dt);

}
void Game::Render() {
	SDL_SetRenderDrawColor(m_renderer, 128, 0, 128, 255);//back ground
	SDL_RenderClear(m_renderer);//double-buffering her yeni render i leminde renererer  temizle

	m_RQ.Clear();
	m_Anim.Draw(m_RQ, 5);

	m_BrickDst.x = static_cast<int>(m_BrickPos.x);
	m_BrickDst.y = static_cast<int>(m_BrickPos.y);
	m_RQ.Add({ m_BrickTex, m_BrickDst, SDL_Rect{ 0,0,0,0 },10 });
	m_RQ.Flush(m_renderer);
	SDL_RenderPresent(m_renderer);


}
void Game::Shutdown() {
	m_Assets.Shutdown();
	if (m_renderer) {
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr; //if there is not renderer it is delete 
	}
	if (m_window) {
		SDL_DestroyWindow(m_window);
		m_window = nullptr; //if there is not window it is delete
	}
	m_Physics.Shutdown();

	SDL_Quit();
}
void Game::Run() {
	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 freq = SDL_GetPerformanceFrequency();
	Uint32 fpsTimer = SDL_GetTicks();
	int frameCnt = 0;
	while (m_isRunning) {
		Uint64 newNow = SDL_GetPerformanceCounter();
		float dt = static_cast<float>(newNow - now) / freq;
		now = newNow;
		frameCnt++;
		Uint32 frameStart = SDL_GetTicks();//oyunun ba lad    framedeki zaman  al r
		ProcessInput();
		Update(dt);
		Render();
		Uint32 elapsed = SDL_GetTicks() - frameStart;
		if (elapsed < FRAME_DELAY) {
			SDL_Delay(FRAME_DELAY - elapsed);
		}
		if (SDL_GetTicks() - fpsTimer >= 1000) {
			std::cout << "fps:" << frameCnt << std::endl;
			frameCnt = 0;
			fpsTimer += 1000;
		}
	}
}

