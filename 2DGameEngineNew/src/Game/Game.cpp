#include "Game.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>
#include "spdlog/spdlog.h"
#include <fstream>

Game::Game() {
	isRunning = false;
	int millisecsPreviousFrame = 0;
	window = NULL;
	renderer = NULL;
	windowWidth = 0;
	windowHeight = 0;

	//unique pointer
	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();

	spdlog::info("Game constructor called");

}

Game::~Game() {
	spdlog::info("Game destructor called");
}

void Game::Initialize() {
	//unable to initialize 
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		spdlog::error("Game destructor called");
		return;
	}
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	//window dimensions
	windowWidth = displayMode.w;//displayMode.w or 800
	windowHeight = displayMode.h;//displayMode.h or 600
	//windowWidth = 800;
	//windowHeight = 600;

	//create a window
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		spdlog::error("Error creating SDL window.");
		return;
	}
	//create the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		spdlog::error("Error creating SDL renderer.");
		return;
	}
	//changes it to real fullscreen
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	isRunning = true;
}

void Game::ProcessInput() {
	SDL_Event sdlEvent;
	//while there is an event pending
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
			//example is closing the window with the x button on the top right
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}
			break;
		}
	}
}

void Game::LoadLevel(int level) {
	//Add the systems that need to be processed in our game
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CollisionSystem>();

	// Adding assets to the asset store
	assetStore->AddTexture(renderer, "cat-image", "./assets/images/fatcat.png");
	assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-tiger-right.png");
	assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
	assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper.png");
	assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
	assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");

	//load the tilemap
	int tileSize = 32; //pixels per tile
	double tileScale = 1.0; //make it larger or smaller
	int mapNumCols = 25;
	int mapNumRows = 20;

	std::fstream mapFile;
	//should probably have some error checking for this
	mapFile.open("./assets/tilemaps/jungle.map");

	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore(); //skipping the comma

			Entity tile = registry->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, srcRectX, srcRectY);
		}
	}
	//close the fstream
	mapFile.close();

	Entity chopper = registry->CreateEntity();
	chopper.AddComponent<TransformComponent>(glm::vec2(100.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(15.0, 15.0)); //the velocity
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
	//10 fps right now
	chopper.AddComponent<AnimationComponent>(2, 10, true);

	Entity radar = registry->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(windowWidth - 74, 10.0), glm::vec2(1.0, 1.0), 0.0);
	radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 2);
	//amount of frames, fps, 
	radar.AddComponent<AnimationComponent>(8, 10, true);

	Entity tank = registry->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(500.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(-30.0, 0.0));
	tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
	tank.AddComponent<BoxColliderComponent>(32, 32);

	Entity truck = registry->CreateEntity();
	truck.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
	truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 1);
	truck.AddComponent<BoxColliderComponent>(32,32);
	


	// Create an entity
	/*Entity cat = registry->CreateEntity();
	cat.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1, 1), 0.0);
	cat.AddComponent<RigidBodyComponent>(glm::vec2(30.0, 0.0));
	cat.AddComponent<SpriteComponent>("cat-image", 500, 500, 1);*/
	//cat.AddComponent<AnimationComponent>(2, 10, true);

	/*Entity truck = registry->CreateEntity();
	truck.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
	truck.AddComponent<SpriteComponent>("truck-image", 100, 100, 1);*/


	


}

void Game::Setup() {

	LoadLevel(1);
	

}

void Game::Update() {
	//TODO: Update game objects
	// 
	//if we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
	//this is not necessarily needed, but I'm going to keep it here anyways
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}

	//the difference in ticks since the last frame, converted to seconds
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

	//store the current frame time
	millisecsPreviousFrame = SDL_GetTicks();

	//update the registry to process the entities that are waiting to be created/deleted
	registry->Update();

	//invoke all the systems that need to update
	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CollisionSystem>().Update();
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	registry->GetSystem<RenderSystem>().Update(renderer, assetStore);

	/*SDL_Surface* surface = IMG_Load("./assets/images/fatcat.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect dstRect = { 10, 10, 32, 32 };
	SDL_RenderCopy(renderer, texture, NULL, &dstRect);*/

	SDL_RenderPresent(renderer);
}

void Game::Run() {
	Setup();
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}