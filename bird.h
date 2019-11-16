#ifndef DEF_BIRD
#define DEF_BIRD

#include <SDL/SDL.h>
#include <iostream>

#include "wall.h"

class Bird{
public:
	Bird(SDL_Surface *screen, int x, int speed);
	~Bird();
	bool isAlive() const;
	void setPosX(int x);
	int getPosY() const;
	int getPosX() const;
	int getPoint() const;
	int getScore() const;
	void increasePoint();
	bool draw(int fps, Wall *target);
	void init();
	void jump();
protected:
	static int m_instance;
	SDL_Surface *m_screen;
	bool alive;
	int m_x, m_point, numero, m_speed, m_score;
	double m_y, forceY;
};

#endif