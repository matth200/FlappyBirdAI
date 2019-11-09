#include <SDL/SDL.h>
#include <iostream>

#include <chrono>
#include <thread>

#include "draw.h"

#define FPS 60.0

using namespace std;
typedef chrono::high_resolution_clock::time_point time_point;


int main(int argc , char *argv[])
{
	if(SDL_Init(SDL_INIT_VIDEO)<0)
		cout << "Error to init video" << endl;
	atexit(SDL_Quit);

	SDL_Surface *screen = 0;
	screen = SDL_SetVideoMode(WIDTH,HEIGHT,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	if(screen==0)
		cout << "Error to init screen" << endl;

	SDL_WM_SetCaption("Flappy Bird",0);

	bool continuer = 1;

	time_point start, end;
	int deltaTime = 0;

	SDL_Event event;

	int x = 0, y = 0;
	while(continuer)
	{
		start = chrono::high_resolution_clock::now();

		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					continuer = 0;
				break; 

				case SDL_KEYDOWN:
				break;

				case SDL_KEYUP:
				break;

				case SDL_MOUSEMOTION:
					x = event.button.x;
					y = event.button.y;
				break;
			}
		}


		SDL_FillRect(screen,0,SDL_MapRGB(screen->format,0,0,0));

		Draw::drawLine(screen,200,200,x,y,SDL_MapRGB(screen->format,255,255,255));
		Draw::drawSquare(screen,200,200,x-200,y-200,SDL_MapRGB(screen->format,200,200,200));

		SDL_Flip(screen);
		end = chrono::high_resolution_clock::now();
		deltaTime = chrono::duration_cast<chrono::milliseconds>(end-start).count();
		//on respecte les FPS spécifié
		if(1000.0/FPS<deltaTime)
			this_thread::sleep_for(chrono::milliseconds(deltaTime-int(1000.0/FPS)));
	}
	return 0;
}
