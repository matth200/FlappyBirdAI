#ifndef DEF_WORLD
#define DEF_WORLD

#include <SDL/SDL.h>
#include <vector>

#include "draw.h"
#include "bird.h"
#include "wall.h"

class World{
public:
	World(SDL_Surface *screen, int nbr_wall, int gap);
	~World();
  	void setBird(Bird *bird);
	bool draw_all(int fps);
protected:
	int m_nbr_wall, m_gap;
	Bird *m_bird;
	std::vector<Wall> walls;
	bool countWall;
	int avancement;
	SDL_Surface *m_screen;
};

#endif