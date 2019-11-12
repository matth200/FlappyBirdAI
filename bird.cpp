#include "bird.h"
using namespace std;

int Bird::m_instance = 0;

Bird::Bird(SDL_Surface *screen,int x):m_screen(screen),m_x(x),m_y(200),forceY(0),m_point(0),alive(1),numero(m_instance)
{
	m_instance++;
}

Bird::~Bird()
{
}

bool Bird::isAlive() const
{
	return alive;
}

void Bird::setPosX(int x)
{
	m_x = x;
}

int Bird::getPosX() const
{
	return m_x;
}

int Bird::getPoint() const
{
	return m_point;
}

void Bird::increasePoint()
{
	m_point++;
}

bool Bird::draw(int fps, Wall *target)
{
	bool collision = false;

	if(alive){
		//on ajoute l'acceleration
		m_y+= forceY;
		//acceleration de chute
		forceY+=600.0/pow(fps,2);
	}
	//stop when pass through the sky
	if(m_y<20)
	{
		m_y=20;
		forceY=0;
	}

	//on affiche et on test si il y a la collision
	collision = drawCircle(m_screen,m_x,m_y,20,SDL_MapRGBA(m_screen->format,(200+numero*55)%255,(100+numero*100)%255,(25+numero*70)%255,100),target);

	//touch the floor
	if(m_y>=HEIGHT-20)
		collision = true;

	if(collision)
		alive = 0; 
	
	return collision; 
}

void Bird::jump()
{
	forceY=-9;
}