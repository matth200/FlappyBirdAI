#include "wall.h"
using namespace std;

Wall::Wall(SDL_Surface *screen, int x, int width, int min_wall, int hole_size):m_screen(screen),m_width(width),m_min_wall(min_wall),m_hole_size(hole_size),m_x(x)
{
	m_top_hole = rand()%(HEIGHT-m_min_wall*2-m_hole_size);
	m_bottom_hole = HEIGHT-(m_top_hole+m_hole_size+m_min_wall*2);
}
Wall::~Wall()
{
}

void Wall::draw(int x)
{
	m_x = x;
	Uint32 color = SDL_MapRGB(m_screen->format,255,255,255);
	Draw::drawRect(m_screen,x,0,m_width,m_min_wall+m_top_hole,color);
	Draw::drawRect(m_screen,x,HEIGHT-1,m_width,-(m_min_wall+m_bottom_hole),color);
}

int Wall::getPosX()
{
	return m_x;
}

double Wall::dist(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x2-x1,2)+pow(y1-y2,2));
}

double Wall::getTopPos(int x, int y, bool a)
{
	if(a==0){
		return dist(m_x,m_min_wall+m_top_hole,x,y);
	}else{
		return dist(m_x+m_width,m_min_wall+m_top_hole,x,y);
	}
}

double Wall::getBottomPos(int x, int y, bool a)
{
	if(a==0){
		return dist(m_x,HEIGHT-1-(m_min_wall+m_bottom_hole),x,y);
	}else{
		return dist(m_x+m_width,HEIGHT-1-(m_min_wall+m_bottom_hole),x,y);
	}
}