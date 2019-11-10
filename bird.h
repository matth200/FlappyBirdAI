#ifndef DEF_BIRD
#define DEF_BIRD

#include <SDL/SDL.h>

#include "draw.h"

class Bird{
public:
	Bird(SDL_Surface *screen, int x);
	~Bird();
	void draw(int fps);
	void jump();
protected:
	SDL_Surface *m_screen;
	int m_x;
	int m_y;
	int m_jump;
};

#endif