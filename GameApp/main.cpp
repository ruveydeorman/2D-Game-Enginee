#include"../Enginee/Game/Game.h"
#include<iostream>
#include<SDL2/SDL.h>

int main(int argc, char* argv[]) {

	Game game;
	if (!game.Init("My Game Enginee", 1280, 720)) {
		return -1;
	}
	game.Run();
	return 0;

}