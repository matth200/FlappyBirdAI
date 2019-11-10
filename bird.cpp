#include "bird.h"
using namespace std;

Bird::Bird(SDL_Surface *screen,int x):m_screen(screen),m_x(x),m_y(200),forceY(0)
{
}
Bird::~Bird()
{
}

void Bird::draw(int fps, Wall *target)
{
	//on ajoute l'acceleration
	m_y+= forceY;
	//acceleration de chute
	forceY+=300.0/pow(fps,2);
	
	bool collision = false;

	collision = drawCircle(m_screen,m_x,m_y,20,SDL_MapRGB(m_screen->format,200,100,25),target);
	if(collision)
		cout << "collision!!" << endl;
}

void Bird::jump()
{
	forceY=-5;
}