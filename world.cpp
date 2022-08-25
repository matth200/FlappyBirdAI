#include "world.h"
using namespace std;

World::World(SDL_Surface *screen, int nbr_wall, int gap):m_screen(screen),m_nbr_wall(nbr_wall),m_gap(gap),m_birds(0)
{
	init();
	m_index = 0;
}

World::~World()
{
}

int World::getDistanceX(int index){
	int x = (*m_birds)[index].getPosX();
	for(vector<Wall>::iterator it(walls.begin());it!=walls.end();it++)
	{
		//si le mur est devant l'oiseau
		if(it->getPosX()>=x+20)
		{
			return it->getPosX()-x-20;
		}
	}
	return -1;
}

int World::getDistanceTop(int index){
	int x = (*m_birds)[index].getPosX();
	for(vector<Wall>::iterator it(walls.begin());it!=walls.end();it++)
	{
		//si le mur est devant ou qu'on est dedans
		if(x-20<=it->getPosX()+80)
		{
			int value = it->getDiffTop((*m_birds)[index].getPosY());

			return value;
		}
	}
	return -1;
}

int World::getDistanceBottom(int index){
	int x = (*m_birds)[index].getPosX();
	for(vector<Wall>::iterator it(walls.begin());it!=walls.end();it++)
	{
		//si le mur est devant ou qu'on est dedans
		if(x-20<=it->getPosX()+80)
		{
			int value = it->getDiffBottom((*m_birds)[index].getPosY());

			return value;
		}
	}
	return -1;
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
	
	//cout << "bird0 distance : " << getDistanceX(0) << " top: " << getDistanceTop(0) << " bottom: " << getDistanceBottom(0) << endl;
	
	//draw each wall and manage collision
	for(vector<Wall>::iterator it(walls.begin());it!=walls.end();it++)
	{

		//the wall goo and we register the avancement
		it->draw(it->getPosX()-((collision)?0:WORLD_SPEED/fps));

		//increase point if the bird pass through a wall
		if(it->getPosX()+80/2<(*m_birds)[0].getPosX()&&!m_countWall)
		{
			int max = 0;
			for(int i(0);i<m_birds->size();i++)
			{
				if((*m_birds)[i].isAlive())
				{
					(*m_birds)[i].increasePoint();
					//get the max
					if((*m_birds)[i].getPoint()>max)
						max = (*m_birds)[i].getPoint();
				}
			}
			cout << max << endl;

			m_countWall = 1;
		}

		//if the wall is not display we remove it and add a new wall at the back
		if(it->getPosX()<-90)
		{
			//remove
			walls.erase(it);

			//nouveau mur avec une distance de gap+80 avec le dernier mur
			walls.push_back(Wall(m_screen,walls[walls.size()-1].getPosX()+m_gap+80));
			m_index++;
			//on enregistre la nouvelle valeur
			if(m_index>levels.size()){
				levels.push_back(walls[walls.size()-1].getRandom());
			}
			//on reprends l'ancienne valeur sinon
			else{
				walls[walls.size()-1].setRandom(levels[m_index-1]);
				//cout << "---" << m_index << "," <<  levels[m_index-1]%50 << endl;
			}
			m_countWall = 0;
		}
	}

	return collision;
}

void World::init()
{
	walls.clear();
	m_countWall = 0;
	for(int i(0);i<m_nbr_wall;i++)
	{
		walls.push_back(Wall(m_screen,WIDTH+(m_gap+80)*i));//+80 pour la taille du mur en largeur

		//si le mur a déjà été capté on remets le random 
		if(levels.size()>i){
			walls[i].setRandom(levels[i]);
		}
		//sinon on enregistre
		else{
			levels.push_back(walls[i].getRandom());
		}
		m_index++;
	}

	if(m_birds!=0)
	{
		for(int i(0);i<(*m_birds).size();i++)
			(*m_birds)[i].init();
	}
}