#include "world.h"
using namespace std;

World::World(SDL_Surface *screen, int nbr_wall, int gap):m_screen(screen),m_nbr_wall(nbr_wall),m_gap(gap)
{
	for(int i(0);i<nbr_wall;i++)
	{
		walls.push_back(Wall(screen,WIDTH+(gap+80)*i));//+80 pour la taille du mur en largeur
	}
}

World::~World()
{
}

void World::setBird(Bird *bird)
{
	m_bird = bird;
}

bool World::draw_all(int fps)
{
	bool collision = m_bird->draw(fps, &(walls[0]));
	for(vector<Wall>::iterator it(walls.begin());it!=walls.end();it++)
	{
		it->draw(it->getPosX()-((collision)?0:100/fps));
		if(it->getPosX()<-90)
		{
			walls.erase(it);
			//nouveau mur avec une distance de gap+80 avec le dernier mur
			walls.push_back(Wall(m_screen,walls[walls.size()-1].getPosX()+m_gap+80));
		}
	}

	return collision;
}