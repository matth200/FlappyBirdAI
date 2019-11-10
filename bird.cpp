#include "bird.h"
using namespace std;

Bird::Bird(SDL_Surface *screen,int x):m_screen(screen),m_x(x),m_y(200),forceY(0),pause(0)
{
}
Bird::~Bird()
{
}

bool Bird::draw(int fps, Wall *target)
{
	if(!pause){
		//on ajoute l'acceleration
		m_y+= forceY;
		//acceleration de chute
		forceY+=300.0/pow(fps,2);
	}

	bool collision = drawCircle(m_screen,m_x,m_y,20,SDL_MapRGB(m_screen->format,200,100,25),target);
	if(collision)
		pause = 1; 
	
	return collision; 
}

void Bird::jump()
{
	forceY=-5;
}