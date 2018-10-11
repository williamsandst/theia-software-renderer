// Theia.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SDL.h>
#include <SDL_render.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include "SDL_image.h"


/*
Structure:

Everything is a graphics object. Starting out, there will only be 1 object.
These objects contain the polygons of the object, the texture info etc
All objects polygons are represented in the polygonMatrix.
This is where the perspective stuff happens.

Draw class: Draws lines, polygons, points etc.
Object class: Contains an object
Polygon struct

*/

//const int width = 1366;
//const int height = 768;
const int width = 800;
const int height = 800;

using namespace std;

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	atexit(SDL_Quit);

	SDL_Window* window = SDL_CreateWindow("Theia - Software Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture* texture = SDL_CreateTexture
	(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height
	);

	SDL_Event event;
	bool running = true;

	//string dir = FileHandler::getRunningDir();
	//TextureManager textureManager;
	//textureManager.dir = dir;

	//ObjectManager objectManager;
	//objectManager.txMng = &textureManager;
	//objectManager.dir = dir;

	/*unsigned char *displayMap = &graphics.frameBuffer.pixelVector[0];
	graphics.frameBuffer.clear();

	graphics.setProjection(PERSPECTIVE);
	graphics.setDrawmode(POLY);
	graphics.setIllumMode(FLAT);
	graphics.setViewport(width, height);
	graphics.toggleBackfaceCulling(true);

	graphics.setTranslation(Vector3f(0, 0, -3));
	graphics.setRotation(Vector3f(3.14, 3.14, 0));
	graphics.setScale(1);*/

	//Debug stuff
	int objectID = 0;
	int drawModeID = 0;

	while (running)
	{
		const Uint64 start = SDL_GetPerformanceCounter();

		//SDL - Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		//Graphics - Clear framebuffer

		//Check for events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				/* Keyboard event */
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE: //Esc closes window
					running = false;
					break;
				}
				break;
			case SDL_KEYUP:
				break;
				/* SDL_QUIT event (window close) */
			case SDL_QUIT:
				running = false;
				break;

			default:
				break;
			}
		}

		//Graphics - Render
		//graphics.renderWireframe();
		//SDL_UpdateTexture(texture, NULL, displayMap, width * 4);

		//SDL - Render screen
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		//Performance
		const Uint64 end = SDL_GetPerformanceCounter();
		const static Uint64 freq = SDL_GetPerformanceFrequency();
		const double seconds = (end - start) / static_cast< double >(freq);
		cout << "Frame time: " << to_string(seconds * 1000.0) << " ms, " << to_string(1 / seconds) << " fps" << endl;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}
//Render an image for testing
/*TexturePtr txrPtr = textureManager.load("guy.tga");
unsigned char* textureMap = (unsigned char*)txrPtr.get()->pixels;
int lineoffset;
for (size_t y = 0; y < height && y < txrPtr->clip_rect.h; y++)
{
int lineoffset = y * txrPtr->pitch;
for (size_t x = 0; x < width && x < txrPtr->clip_rect.w; x++)
{
displayMap[y*width*4 + x*4] = textureMap[lineoffset + x * 3];
displayMap[y*width * 4 + x * 4 + 1] = textureMap[lineoffset + x * 3 + 1];
displayMap[y*width * 4 + x * 4 + 2] = textureMap[lineoffset + x * 3 + 2];
//displayMap[y*width * 4 + x * 4 + 1] = textureMap[lineoffset + x + 1];
//displayMap[y*width * 4 + x * 4 + 2] = textureMap[lineoffset + x + 2];
}
}*/