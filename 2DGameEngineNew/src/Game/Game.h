#ifndef GAME_H
#define GAME_H

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include <SDL.h>

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
	bool isRunning;
	int millisecsPreviousFrame = 0;
	SDL_Window* window;
	SDL_Renderer* renderer;

	//da manager
	std::unique_ptr<Registry> registry; //Registry* registry
	std::unique_ptr<AssetStore> assetStore;


public:
	Game();
	~Game();
	void Initialize();
	void Run();
	//important for frame by frame looping
	void Setup();
	void LoadLevel(int level);
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

	int windowWidth;
	int windowHeight;

};

#endif