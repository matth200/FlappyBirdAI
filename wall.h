#ifndef DEF_WALL
#define DEF_WALL

#include <SDL/SDL.h>
#include <cmath>
#include "draw.h"

class Wall
{
public:
	Wall(SDL_Surface *screen, int min_wall = 10, int hole_size = 100);
	~Wall();

protected:
	int m_min_wall, m_hole_size; 
}

#endif