#ifndef DEF_BIRD
#define DEF_BIRD

#include <SDL/SDL.h>

class Bird{
public:
	Bird(SDL_Surface *screen);
	~Bird();
	void draw();
protected:
	SDL_Surface *m_screen;
};

#endif