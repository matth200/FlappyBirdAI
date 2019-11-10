#ifndef DEF_WALL
#define DEF_WALL

#include <SDL/SDL.h>
#include <cmath>

//random librairy
#include <cstdlib>
#include <ctime>

#include "draw.h"

struct DoubleDist{
	double dist1;
	double dist2;
};

class Wall
{
public:
	Wall(SDL_Surface *screen, int width = 80, int min_wall = 10, int hole_size = 100);
	~Wall();
	void draw(int x);
	DoubleDist getTopPos(bool a = 0);
	DoubleDist getBottomPos(bool a = 0);

protected:
	int m_min_wall, m_hole_size, m_width; 
	SDL_Surface *m_screen;
	int m_top_hole, m_bottom_hole;
};

#endif