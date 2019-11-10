#ifndef DEF_WORLD
#define DEF_WORLD

#include <SDL/SDL.h>
#include <vector>

#include "draw.h"
#include "wall.h"

class World{
public:
	World(SDL_Surface *screen, int nbr_wall);
	~World();
  	void setBird(&Bird bird);
	void draw_all();
protected:
	int m_nbr_wall;
	Bird m_bird;
	vector<Wall> walls;
	int avancement;
	SDL_Surface *m_screen;
};

#endif