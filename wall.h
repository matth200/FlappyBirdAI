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
	Wall(SDL_Surface *screen,int x, int width = 80, int min_wall = 80, int hole_size = 150);
	~Wall();
	void draw(int x);
	int getPosX();
	bool getCollision(int x, int y);
	double getTopDist(int x, int y, bool a);
	double getBottomDist(int x, int y, bool a);
	int getDiffTop(int y) const;
	int getDiffBottom(int y) const;

protected:
	double dist(int x1, int y1, int x2, int y2);
	int m_x;
	int m_min_wall, m_hole_size, m_width; 
	SDL_Surface *m_screen;
	int m_top_hole, m_bottom_hole;
};

//surcharge pour detection de collision
bool setPixel(SDL_Surface *screen, int x, int y, Uint32 color, Wall *w);
bool drawLine(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color, Wall *w);
bool drawCircle(SDL_Surface *screen, int x, int y, int r, Uint32 color, Wall *w, int resolution = 30);

#endif