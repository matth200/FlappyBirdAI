#ifndef DEF_WALL
#define DEF_WALL

#include <SDL/SDL.h>
#include <cmath>

//random librairy
#include <cstdlib>
#include <ctime>

#include "draw.h"

class Wall
{
public:
	Wall(SDL_Surface *screen,int x, int width = 80, int min_wall = 10, int hole_size = 150);
	~Wall();
	void draw(int x);
	int getPosX();
	double getTopPos(int x, int y, bool a);
	double getBottomPos(int x, int y, bool a);

protected:
	double dist(int x1, int y1, int x2, int y2);
	int m_x;
	int m_min_wall, m_hole_size, m_width; 
	SDL_Surface *m_screen;
	int m_top_hole, m_bottom_hole;
};

#endif