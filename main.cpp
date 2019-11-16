#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include <iostream>

#include <chrono>
#include <thread>
#include <string>
#include <vector>


#include "draw.h"
#include "wall.h"
#include "bird.h"
#include "world.h"

#include "m_learning.h"

#define FPS 30.0
#define NBR_POPULATION 10
#define MAX_VIEW 400.0

using namespace std;
typedef chrono::high_resolution_clock::time_point time_point;

struct Duet
{
	int pos;
	int score;
};

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

	TTF_Font *police = TTF_OpenFont("pixel_font.ttf",300);

	SDL_Surface *Label = 0, *Title = TTF_RenderText_Blended(police,"Perdu",SDL_Color({255,255,255}));

	TTF_CloseFont(police);
	police = TTF_OpenFont("pixel_font.ttf",20);

	SDL_Rect pos_label, pos_title;
	pos_label.x = 20;
	pos_label.y = 50;

	pos_title.x = 50;
	pos_title.y = 200;

	World world(screen,4,350);

	vector<MachineLearning> brains;
	vector<Bird> listeBirds;

	for(int i(0);i<NBR_POPULATION;i++)
	{
		MachineLearning m(3);
		m.addColumn(6);
		m.addColumn(1);

		m.setWeightRandom(50,50);
		
		brains.push_back(m);

		listeBirds.push_back(Bird(screen,200,WORLD_SPEED));
	}
	world.setBirds(&listeBirds);


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
							//only if we want to play without IA
							//listeBirds[0].jump();
						break;
					}
				break;
			}
		}
		SDL_FillRect(screen,0,SDL_MapRGB(screen->format,0,0,0));
		 
		Label = TTF_RenderText_Blended(police,(string("Point(s): ")+to_string(listeBirds[0].getPoint())).c_str(),SDL_Color({255,255,255}));
		SDL_BlitSurface(Label,NULL,screen,&pos_label);

		//calcul NeuralNetwork
		for(int i(0);i<NBR_POPULATION;i++)
		{
			double dX = world.getDistanceX(i), dTop = world.getDistanceTop(i)+MAX_VIEW/2, dBottom = world.getDistanceBottom(i)+MAX_VIEW/2;

			//max
			dX = (dX>MAX_VIEW)?MAX_VIEW:dX;
			dTop = (dTop>MAX_VIEW)?MAX_VIEW:dTop;
			dBottom = (dBottom>MAX_VIEW)?MAX_VIEW:dBottom;

			//min
			dTop = (dTop<0)?0:dTop;
			dBottom = (dBottom<0)?0:dBottom;

			//range(0,1)
			dX/=MAX_VIEW;
			dTop/=MAX_VIEW;
			dBottom/=MAX_VIEW;

			double data[3];
			data[0] = dX;
			data[1] = dTop;
			data[2] = dBottom;

			brains[i].setInput(data);
			brains[i].calcul();

			if(brains[i].getOutput(0)>=0.5)
			{
				listeBirds[i].jump();
			}

			cout << "bird n°" << i << " output=" << brains[i].getOutput(0) << endl;
		}


		//draw the title "perdu" if it's lost
		if(world.draw_all(FPS))
		{
			//population all dead
			SDL_BlitSurface(Title,NULL,screen,&pos_title);


			Duet nullDuet;
			nullDuet.pos = -1;
			nullDuet.score = 0;

			vector<Duet> tab_score(NBR_POPULATION,nullDuet);

			//on récupére et on trie les éléments
			for(int i(0);i<NBR_POPULATION;i++)
			{
				Duet a;
				a.pos = i;
				a.score = listeBirds[i].getScore();

				//trie des éléments
				for(int j(0);j<tab_score.size();j++)
				{
					if(tab_score[j].score<a.score)
					{
						Duet precedent;
						for(int z(j);z<tab_score.size();z++)
						{
							precedent = tab_score[z];
							tab_score[z] = a;
							a = precedent;
						}
						break;
					}
				}
			}

			for(int i(0);i<NBR_POPULATION;i++)
			{
				cout << "bird n°" << tab_score[i].pos << " score=" << tab_score[i].score << endl;
			}
			cout << "---------" << endl;

			//on enregistre le premier si il dépasse 1600px
			if(tab_score[0].score>=1600)
				brains[tab_score[0].pos].saveTraining("goodBrain.ml");
		}

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
