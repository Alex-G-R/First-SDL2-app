#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Utils.hpp"

int main(int argc, char* args[])
{
	if(SDL_Init(SDL_INIT_VIDEO) > 0)
		std::cout << "SDL_init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;

	if(!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_init HAS FAILED. ERROR: " << SDL_GetError() << std::endl;

	// Start up the window, name it n' stuff
	RenderWindow window("Super cool stuff", 1280, 720);
	int windowRefreshRate = window.getRefreshRate();


	SDL_Texture* testTexture = window.loadTexture("res/gfx/texture.png");

	std::vector<Entity> entities;
	{
		Entity tile1(Vector2f(200, 200), testTexture);
		entities.emplace_back(tile1);
	}

	// Game loop
	bool gameRunning = true;
	SDL_Event event;

	const float timeStep = 0.01f;
	float accumulator = 0.0f;
	float currentTime = utils::hireTimeInSeconds();

	while(gameRunning)
	{
		int startTicks = SDL_GetTicks();

		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;

		currentTime = newTime;

		accumulator += frameTime;

		while(accumulator >= timeStep)
		{
			while(SDL_PollEvent(&event)){
				if (event.type == SDL_QUIT)
					gameRunning = false;
			}

			accumulator -= timeStep;
		}

		const float alpha = accumulator / timeStep;

		window.clear();

		for(Entity& e : entities){
			window.render(e);
		}
		window.display();

		int frameTicks = SDL_GetTicks() - startTicks;

		if(frameTicks < 1000/window.getRefreshRate())
			SDL_Delay(1000/window.getRefreshRate() - frameTicks);

	}

	// Safely kill the bastard
	window.cleanUp();
	SDL_Quit();
	return 0;	
}