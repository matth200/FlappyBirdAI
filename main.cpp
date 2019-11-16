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
#define MUTATE_RATE 0.08
#define RANDOM_VALUE 50
#define NEW_RANDOM_BRAIN 1

using namespace std;
typedef chrono::high_resolution_clock::time_point time_point;

struct Duet
{
	int pos;
	int score;
};

void setAdn(MachineLearning &m, vector<double> &adn);
void getAdn(MachineLearning &m, vector<double> &adn);
void mutateAdn(vector<double> &adn);
int selectionParent(vector<Duet> &tab);
void makeDammnBaby(vector<double> &adnMom, vector<double> &adnDad, vector<double> &adnBaby);

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
	int generation = 1;

	vector<MachineLearning> brains;
	vector<Bird> listeBirds;

	for(int i(0);i<NBR_POPULATION;i++)
	{
		MachineLearning m(3);
		m.addColumn(6);
		m.addColumn(1);

		m.setWeightRandom(RANDOM_VALUE,RANDOM_VALUE);
		
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

		//on récupére le score d'oiseaux max
		int pointMax = 0;
		for(int i(0);i<listeBirds.size();i++)
		{
			if(listeBirds[i].getPoint()>pointMax)
			{
				pointMax = listeBirds[i].getPoint();
			}
		}
		 
		Label = TTF_RenderText_Blended(police,(string("Point(s): ")+to_string(pointMax)+" Generation: "+to_string(generation)).c_str(),SDL_Color({255,255,255}));
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
		}


		//draw the title "perdu" if it's lost
		if(world.draw_all(FPS))
		{
			//SDL_BlitSurface(Title,NULL,screen,&pos_title)

			//population all dead
			Duet nullDuet;
			nullDuet.pos = -1;
			nullDuet.score = 0;

			//tableau de score
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

			cout << "generation ---> " << generation << endl; 
			for(int i(0);i<NBR_POPULATION;i++)
			{
				cout << "bird n°" << tab_score[i].pos << " score=" << tab_score[i].score << endl;
			}
			cout << "\\---------\\" << endl;

			//on enregistre le premier si il dépasse 1600px
			if(tab_score[0].score>=1600)
				brains[tab_score[0].pos].saveTraining("goodBrain.ml");

			//new generation
			generation++;
			vector<MachineLearning> newBrains;

			//first
			newBrains.push_back(brains[tab_score[0].pos]);
			
			while(newBrains.size()<brains.size()-NEW_RANDOM_BRAIN)
			{
				int momIndex = selectionParent(tab_score), dadIndex;
				//on évite que le père et la mère soit la même personne 
				while((dadIndex = selectionParent(tab_score))==momIndex);
				//cout << "Dad: " << dadIndex << " Mom: " << momIndex << endl;

				//cout << "getAdn parents" << endl;
				//preparation adn parent
				vector<double> adnMom, adnDad;
				getAdn(brains[momIndex],adnMom);
				getAdn(brains[dadIndex],adnDad);

				//cout << "makeDammnBaby()" << endl;
				//get adnBaby
				vector<double> adnBaby; 
				makeDammnBaby(adnMom,adnDad,adnBaby);

				//cout << "mutate" << endl;
				//mutate the adn
				mutateAdn(adnBaby);

				//cout << "setAdn() babyBrain" << endl;
				//make the babyBrain with it
				MachineLearning babyBrain = brains[0];
				setAdn(babyBrain,adnBaby);
				newBrains.push_back(babyBrain);
			}

			//cout << "Random" << endl;
			//new random brain
			for(int i(0);i<NEW_RANDOM_BRAIN;i++)
			{
				MachineLearning m = brains[0];
				m.setWeightRandom(RANDOM_VALUE,RANDOM_VALUE);
				newBrains.push_back(m);
			}
			//cout << "Size --> " << newBrains.size() << endl;

			//on place la nouvelle population
			brains.clear();
			brains.resize(0);
			brains = newBrains;

			cout << "world init()" << endl;

			//start new game
			world.init();
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


void setAdn(MachineLearning &m, vector<double> &adn)
{
	int sizeAdn = 0;
	//vérification si même taille
	for(int l(1);l<m.getNumberColumn();l++)
	{
		for(int j(0);j<m.getNetwork(l)->get_number_neuron();j++)
		{
			for(int i(0);i<m.getNetwork(l)->get_neuron(j)->numberConnection();i++)
			{
				sizeAdn++;
			}
			sizeAdn++;
		}
	}

	//si meme taille on place tout dans le réseaux neuronaux
	if(adn.size()==sizeAdn)
	{
		int index = 0;
		for(int l(1);l<m.getNumberColumn();l++)
		{
			for(int j(0);j<m.getNetwork(l)->get_number_neuron();j++)
			{
				for(int i(0);i<m.getNetwork(l)->get_neuron(j)->numberConnection();i++)
				{
					m.getNetwork(l)->get_neuron(j)->set_weight(i,adn[index]);
					index++;
				}
				m.getNetwork(l)->get_neuron(j)->set_bias(adn[index]);
				index++;
			}
		}
	}
	else{
		cout << "Error to setAdn()" << endl;
	}

}

void getAdn(MachineLearning &m, vector<double> &adn)
{
	adn.clear();
	for(int l(1);l<m.getNumberColumn();l++)
	{
		for(int j(0);j<m.getNetwork(l)->get_number_neuron();j++)
		{
			for(int i(0);i<m.getNetwork(l)->get_neuron(j)->numberConnection();i++)
			{
				adn.push_back(m.getNetwork(l)->get_neuron(j)->get_weight(i));
			}
			adn.push_back(m.getNetwork(l)->get_neuron(j)->get_bias());
		}
	}
}

void mutateAdn(vector<double> &adn)
{
	for(int i(0);i<adn.size();i++)
	{
		if(rand()%100<MUTATE_RATE*100)
		{
			adn[i] = rand()%(RANDOM_VALUE*1000)/1000.0-RANDOM_VALUE/2.0;
		}
	}
}

void makeDammnBaby(vector<double> &adnMom, vector<double> &adnDad, vector<double> &adnBaby)
{
	int index = rand()%(adnMom.size()-1)+1;
	for(int i(0);i<adnMom.size();i++)
	{
		if(i<index){
			adnBaby.push_back(adnMom[i]);
		}else{
			adnBaby.push_back(adnDad[i]);
		}
	}
}

int selectionParent(vector<Duet> &tab)
{
	int sumTotal = 0;
	for(int i(0);i<tab.size();i++)
	{
		sumTotal += tab[i].score;
	}

	int arrow = rand()%sumTotal;
	int sum = 0;
	for(int i(0);i<tab.size();i++)
	{
		if(arrow<sum)
		{
			return i;
		}
		sum+=tab[i].score;
	}
	return 0;
}