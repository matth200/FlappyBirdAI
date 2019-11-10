#include "bird.h"
using namespace std;

Bird::Bird(SDL_Surface *screen,int x):m_screen(screen),m_x(x),m_y(200),m_jump(0),forceY(0)
{
}
Bird::~Bird()
{
}

void Bird::draw(int fps)
{
	m_y+= forceY;
	forceY+=300.0/pow(fps,2);
	
	Draw::drawCircle(m_screen,m_x,m_y,20,SDL_MapRGB(m_screen->format,200,100,25));
}

void Bird::jump()
{
	m_jump += 75;
	forceY=-5;
}