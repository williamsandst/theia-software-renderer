// Theia.cpp : Defines the entry point for the console application.
//
#include <SDL.h>
#include <SDL_render.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

#include "Graphics.h"
#include "FileHandler.h"
#include "TextureManager.h"
#include "ObjectManager.h"

/*
Theia - A Software Renderer written in C++ using Eigen and SDL

Dependencies: 
	Eigen, for fast Matrix and Vector operations.
	SDL, for displaying pixels on the screen and various input
	SDL_Image, for loading image files from the disk into SDL_Surface objects
Description:
	Theia is a Software Renderer which supports Vertex and Fragment shaders and several passes.
	The program is supposed to be similiar to OpenGL, but due to optimization reasons
	the data structures vary a bit, as there is no GSL and everything needs to run on the CPU.

Todo:
	Features:
		Skybox
		First person camera
		Scene graph
		Particles
	Performance:
		Multithreading (quadrant or polygon-wise?)
	Fixes:
		Fix the projection stage. Something wrong with the viewport transform (stretching at non-square res)
		and Z-values are odd. Make them between 0-1 instead?
		Fix normals?
		Weird Z values, should probably be fixed somehow. Z-depth works now at least, but it is buggy. ->
		something to do with projection matrix not working as it should. Fun times!
		Something very weird happens when the object gets too close to the camera. Seems like a texture error?
		But that should be impossible. Weird...


*/

//const int width = 1920;
//const int height = 1080;
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

	Graphics g = Graphics();
	g.setScreenSize(width, height);

	string dir = FileHandler::getRunningDir();
	TextureManager textureManager;
	textureManager.dir = dir;

	ObjectManager objectManager;
    objectManager.txMng = &textureManager;
	objectManager.dir = dir;

	//Interpolation test
	//g.addToObjectToPipeline(objectManager.load("triangle"));
    g.addToObjectToPipeline(objectManager.load("cube"));
	//g.addToObjectToPipeline(objectManager.load("transparencytest3"));
	//g.addToObjectToPipeline(objectManager.load("transparencytest3"));
	//g.addToObjectToPipeline(objectManager.load("earth", true));
	//g.addToObjectToPipeline(objectManager.load("transparentcube"));
	//g.addToObjectToPipeline(objectManager.load("diablo"));
	//g.worldObjects[1]->scale = 0.5;
	//g.worldObjects[1]->translation[2] += -1;
	//g.worldObjects[1]->updateTranslationMatrix();
	//g.addToObjectToPipeline(objectManager.load("transparentcube"));
	//g.addToObjectToPipeline(objectManager.load("floor"));
	//Something wrong with the human model, memory overflow
	//g.addToObjectToPipeline(objectManager.load("human", true));
	//g.addToObjectToPipeline(objectManager.load("head"));

	cout << "Finished adding objects to pipeline" << endl;

	g.setup();

	//Debug stuff
	int objectID = 0;
	int drawModeID = 0;

	g.clearDisplaybuffer();

	int frameCount = 0;

	while (running)
	{
		const Uint64 start = SDL_GetPerformanceCounter();


		//SDL - Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//Graphics - Clear framebuffer
		g.clearDisplaybuffer();

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
				case SDL_SCANCODE_D:
					g.view.move(0.01*g.view.scale, 0, 0);
					break;
				case SDL_SCANCODE_A:
					g.view.move(-0.01*g.view.scale, 0, 0);
					break;
				case SDL_SCANCODE_W:
					g.view.move(0, -0.01*g.view.scale, 0);
					break;
				case SDL_SCANCODE_S:
					g.view.move(0, 0.01*g.view.scale, 0);
					break;
				case SDL_SCANCODE_Q:
					g.view.move(0, 0, -0.01*g.view.scale);
					break;
				case SDL_SCANCODE_E:
					g.view.move(0, 0, 0.01*g.view.scale);
					break;
				case SDL_SCANCODE_Z: //Rotate
					g.view.rotate(0.03, 0, 0);
					break;
				case SDL_SCANCODE_X:
					g.view.rotate(0, 0.03, 0);
					break;
				case SDL_SCANCODE_C:
					g.view.rotate(0, 0, 0.03);
					break;
				case SDL_SCANCODE_M: //Change Draw Mode
					g.drawMode = DrawMode((g.drawMode + 1) % 3);
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
		g.renderMainView();

		SDL_UpdateTexture(texture, NULL, &g.screen.outputBuffer[0], width * 4);

		//SDL - Render screen
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		//Performance
		const Uint64 end = SDL_GetPerformanceCounter();
		const static Uint64 freq = SDL_GetPerformanceFrequency();
		const double seconds = (end - start) / static_cast< double >(freq);
		if (frameCount % 120 == 0)
			cout << "Frame time: " << to_string(seconds * 1000.0) << " ms, " << to_string(1 / seconds) << " fps" << endl;
		frameCount++;
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