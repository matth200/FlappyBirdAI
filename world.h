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
  	void setBirds(std::vector<Bird> *birds);
  	int getDistanceX(int index);
  	int getDistanceTop(int index);
  	int getDistanceBottom(int index);
	bool draw_all(int fps);

protected:
	int m_nbr_wall, m_gap;
	std::vector<Wall> walls;
	bool m_countWall;
	std::vector<Bird> *m_birds;
	int avancement;
	SDL_Surface *m_screen;
};

#endif