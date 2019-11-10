#ifndef DEF_BIRD
#define DEF_BIRD

#include <SDL/SDL.h>
#include <iostream>

#include "wall.h"

class Bird{
public:
	Bird(SDL_Surface *screen, int x);
	~Bird();
	int getPosX() const;
	int getPoint() const;
	void increasePoint();
	bool draw(int fps, Wall *target);
	void jump();
protected:
	SDL_Surface *m_screen;
	int m_x;
	int m_point;
	double m_y; 
	bool pause;
	double forceY;
};

#endif