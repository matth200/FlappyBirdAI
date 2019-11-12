#include "world.h"
using namespace std;

World::World(SDL_Surface *screen, int nbr_wall, int gap):m_screen(screen),m_nbr_wall(nbr_wall),m_gap(gap),m_countWall(0)
{
	for(int i(0);i<nbr_wall;i++)
	{
		walls.push_back(Wall(screen,WIDTH+(gap+80)*i));//+80 pour la taille du mur en largeur
	}
}

World::~World()
{
}


int World::getDistanceX(int index){
	int x = (*m_birds)[index].getPosX();
	for(vector<Wall>::iterator it(walls.begin());it!=walls.end();it++)
	{
		if(it->getPosX()>=(*m_birds)[0].getPosX()+20)
		{
			return it->getPosX()-x-20;
		}
	}
}
int World::getDistanceTop(int index){

}
int World::getDistanceBottom(int index){

}

void World::setBirds(vector<Bird> *birds)
{
	m_birds = birds;
}

bool World::draw_all(int fps)
{
	bool collision = 1;
	for(int i(0);i<m_birds->size();i++)
	{
		collision = ((*m_birds)[i].draw(fps, &(walls[0])))?collision:0;
	}
	cout << "bird0 distance : " << getDistanceX(0) << endl;
	//draw each wall and manage collision
	for(vector<Wall>::iterator it(walls.begin());it!=walls.end();it++)
	{
		int speed = 200;
		it->draw(it->getPosX()-((collision)?0:speed/fps));

		//increase point if the bird pass through a wall
		if(it->getPosX()+80/2<(*m_birds)[0].getPosX()&&!m_countWall)
		{
			for(int i(0);i<m_birds->size();i++)
			{
				if((*m_birds)[i].isAlive())
				{
					(*m_birds)[i].increasePoint();
				}
			}
			cout << (*m_birds)[0].getPoint() << endl;

			m_countWall = 1;
		}

		//if the wall is not display we remove it and add a new wall at the back
		if(it->getPosX()<-90)
		{
			//remove
			walls.erase(it);

			//nouveau mur avec une distance de gap+80 avec le dernier mur
			walls.push_back(Wall(m_screen,walls[walls.size()-1].getPosX()+m_gap+80));
			m_countWall = 0;
		}
	}

	return collision;
}