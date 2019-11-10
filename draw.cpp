#include "draw.h"

using namespace std;

void Draw::setPixel(SDL_Surface *screen, int x, int y, Uint32 color)
{
	if(x>=0&&y>=0&&x<WIDTH&&y<HEIGHT)
		*((Uint32*)(screen->pixels)+x+y*screen->w) = color;
}

void Draw::drawLine(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color)
{
	int deltaX = x2-x1, deltaY = y1-y2;
	double k = sqrt(pow(deltaX,2)+pow(deltaY,2));
	
	for(int i(0);i<k;i++)
	{
		setPixel(screen,x1+deltaX/k*(k-i),y1-deltaY/k*(k-i),color);
	}
}

void Draw::drawRect(SDL_Surface *screen, int x, int y, int w, int h, Uint32 color)
{
	drawLine(screen,x,y,x+w,y,color);
	drawLine(screen,x+w,y,x+w,y+h,color);
	drawLine(screen,x+w,y+h,x,y+h,color);
	drawLine(screen,x,y+h,x,y,color);
}

void Draw::drawCircle(SDL_Surface *screen, int x, int y, int r, Uint32 color, int resolution)
{
	//setPixel(screen,x,y,color);

	double angle_base = 2*M_PI/double(resolution);

	for(int i(0);i<resolution;i++)
	{
		drawLine(screen,x+sin(angle_base*(i+1))*r,y+cos(angle_base*(i+1))*r,x+sin(angle_base*(i+2))*r,y+cos(angle_base*(i+2))*r,color);
	}
}