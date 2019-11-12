#ifndef DEF_BIRD
#define DEF_BIRD

#include <SDL/SDL.h>
#include <iostream>

#include "wall.h"

class Bird{
public:
	Bird(SDL_Surface *screen, int x);
	~Bird();
	bool isAlive() const;
	void setPosX(int x);
	int getPosX() const;
	int getPoint() const;
	void increasePoint();
	bool draw(int fps, Wall *target);
	void jump();
protected:
	SDL_Surface *m_screen;
	bool alive;
	int m_x;
	int m_point;
	double m_y; 
	double forceY;
};

#endif