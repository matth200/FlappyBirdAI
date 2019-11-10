#include "world.h"
using namespace std;

World::World(SDL_Surface *screen, int nbr_wall, int gap):m_screen(screen),m_nbr_wall(nbr_wall),m_gap(gap),countWall(0)
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
	//draw bird and notice about the collision
	bool collision = m_bird->draw(fps, &(walls[0]));

	//draw each wall and manage collision
	for(vector<Wall>::iterator it(walls.begin());it!=walls.end();it++)
	{
		it->draw(it->getPosX()-((collision)?0:200/fps));

		//increase point if the bird pass through a wall
		if(it->getPosX()+80/2<m_bird->getPosX()&&!countWall)
		{
			m_bird->increasePoint();
			cout << m_bird->getPoint() << endl;
			countWall = 1;
		}

		//if the wall is not display we remove it and add a new wall at the back
		if(it->getPosX()<-90)
		{
			//remove
			walls.erase(it);

			//nouveau mur avec une distance de gap+80 avec le dernier mur
			walls.push_back(Wall(m_screen,walls[walls.size()-1].getPosX()+m_gap+80));
			countWall = 0;
		}
	}

	return collision;
}