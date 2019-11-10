#include "wall.h"
using namespace std;

Wall::Wall(SDL_Surface *screen, int width, int min_wall, int hole_size):m_screen(screen),m_width(width),m_min_wall(min_wall),m_hole_size(hole_size)
{
	m_top_hole = rand()%(HEIGHT-m_min_wall*2-m_hole_size);
	m_bottom_hole = HEIGHT-(m_top_hole+m_hole_size+m_min_wall*2);
}
Wall::~Wall()
{
}

void Wall::draw(int x)
{
	Uint32 color = SDL_MapRGB(m_screen->format,255,255,255);
	Draw::drawRect(m_screen,x,0,m_width,m_min_wall+m_top_hole,color);
	Draw::drawRect(m_screen,x,HEIGHT-1,m_width,-(m_min_wall+m_bottom_hole),color);
}
Pos Wall::getTopPos(bool a)
{

}

Pos Wall::getBottomPos(bool a)
{

}