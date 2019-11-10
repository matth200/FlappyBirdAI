#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include <iostream>

#include <chrono>
#include <thread>


#include "draw.h"
#include "wall.h"
#include "bird.h"
#include "world.h"

#define FPS 30.0

using namespace std;
typedef chrono::high_resolution_clock::time_point time_point;


int main(int argc , char *argv[])
{
	if(SDL_Init(SDL_INIT_VIDEO)<0)
		cout << "Error to init video" << endl;
	atexit(SDL_Quit);

	if(TTF_Init()<0)
		cout << "Error to init sdl_ttf" << endl;
	atexit(TTF_Quit);

	srand(time(0));

	SDL_Surface *screen = 0;
	screen = SDL_SetVideoMode(WIDTH,HEIGHT,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	if(screen==0)
		cout << "Error to init screen" << endl;

	SDL_WM_SetCaption("Flappy Bird",0);

	//variable to the loop
	bool continuer = 1;
	time_point start, end;
	int deltaTime = 0;

	TTF_Font *police = TTF_OpenFont("pixel_font.ttf",20);
	SDL_Surface *Label = 0;
	SDL_Rect pos_label;
	pos_label.x = 100;
	pos_label.y = 100;

	World world(screen,4,300);

	Bird bird(screen,200);
	world.setBird(&bird);

	SDL_Event event;
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
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							continuer = 0;
						break;

						case SDLK_SPACE:
							bird.jump();
						break;
					}
				break;
			}
		}
		SDL_FillRect(screen,0,SDL_MapRGB(screen->format,0,0,0));
		
		Label = TTF_RenderText_Solid(police,"Testttt",SDL_Color({255,255,255}));
		SDL_BlitSurface(screen,NULL,Label,&pos_label);

		//draw
		world.draw_all(FPS);

		SDL_Flip(screen);
		SDL_FreeSurface(Label);
		end = chrono::high_resolution_clock::now();
		deltaTime = chrono::duration_cast<chrono::milliseconds>(end-start).count();
		//on respecte les FPS spécifié
		if(1000.0/FPS>deltaTime)
			this_thread::sleep_for(chrono::milliseconds(int(1000.0/FPS)-deltaTime));
		//deltaTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()-start).count();
		//cout << "FPS : " << 1000.0/deltaTime << " deltaTime : " << deltaTime << endl;
	}
	TTF_CloseFont(police);
	return 0;
}
