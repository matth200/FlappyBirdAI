#ifndef DEF_DRAW
#define DEF_DRAW

#include <SDL/SDL.h>
#include <cmath>

#define WIDTH 700
#define HEIGHT 700

class Draw{
public:
	//fonction de dessin 
	static void setPixel(SDL_Surface *screen, int x, int y, Uint32 color);
	static void drawLine(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color);
	static void drawSquare(SDL_Surface *screen, int x, int y, int w, int h, Uint32 color);
};

#endif