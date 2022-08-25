#include "wall.h"
using namespace std;

Wall::Wall(SDL_Surface *screen, int x, int width, int min_wall, int hole_size):m_screen(screen),m_width(width),m_min_wall(min_wall),m_hole_size(hole_size),m_x(x)
{
	random = rand();
	m_top_hole = random%(HEIGHT-m_min_wall*2-m_hole_size);
	m_bottom_hole = HEIGHT-(m_top_hole+m_hole_size+m_min_wall*2);
}
Wall::~Wall()
{
}

int Wall::getRandom() const
{
	return random;
}

void Wall::setRandom(int rd)
{
	random = rd;
	m_top_hole = random%(HEIGHT-m_min_wall*2-m_hole_size);
	m_bottom_hole = HEIGHT-(m_top_hole+m_hole_size+m_min_wall*2);
}

void Wall::draw(int x)
{
	m_x = x;
	Uint32 color = SDL_MapRGB(m_screen->format,255,255,255);
	Draw::drawRect(m_screen,x,0,m_width,m_min_wall+m_top_hole,color);
	Draw::drawRect(m_screen,x,HEIGHT-1,m_width,-(m_min_wall+m_bottom_hole),color);
}

bool Wall::getCollision(int x, int y)
{
	//top wall collision
	if(m_x<=x&&x<=m_x+m_width&&0<=y&&y<=m_min_wall+m_top_hole)
		return true;
	//bottom wall collision
	if(m_x<=x&&x<m_x+m_width&&m_min_wall+m_top_hole+m_hole_size<=y&&y<=HEIGHT-1)
		return true;
	return false;
}

int Wall::getPosX()
{
	return m_x;
}

double Wall::dist(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x2-x1,2)+pow(y1-y2,2));
}

int Wall::getDiffTop(int y) const
{
	return (m_min_wall+m_top_hole)-y+20;
}

int Wall::getDiffBottom(int y) const
{
	return HEIGHT-(m_min_wall+m_bottom_hole)-y-20;
}

double Wall::getTopDist(int x, int y, bool a)
{
	if(a==0){
		return dist(m_x,m_min_wall+m_top_hole,x,y);
	}else{
		return dist(m_x+m_width,m_min_wall+m_top_hole,x,y);
	}
}

double Wall::getBottomDist(int x, int y, bool a)
{
	if(a==0){
		return dist(m_x,HEIGHT-1-(m_min_wall+m_bottom_hole),x,y);
	}else{
		return dist(m_x+m_width,HEIGHT-1-(m_min_wall+m_bottom_hole),x,y);
	}
}


//surcharge avec detection de collision
bool setPixel(SDL_Surface *screen, int x, int y, Uint32 color, Wall *w)
{
	if(x>=0&&y>=0&&x<WIDTH&&y<HEIGHT)
		*((Uint32*)(screen->pixels)+x+y*screen->w) = color;

	return w->getCollision(x,y); 
}

bool drawLine(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color, Wall *w)
{
	int deltaX = x2-x1, deltaY = y1-y2;
	double k = sqrt(pow(deltaX,2)+pow(deltaY,2));
	
	bool collision = false;

	for(int i(0);i<k;i++)
	{
		collision = (setPixel(screen,x1+deltaX/k*(k-i),y1-deltaY/k*(k-i),color,w))?true:collision;
	}
	return collision;
}

bool drawCircle(SDL_Surface *screen, int x, int y, int r, Uint32 color, Wall *w, int resolution)
{
	//setPixel(screen,x,y,color);

	double angle_base = 2*M_PI/double(resolution);//360°/nbr_d'angle

	bool collision = false;

	for(int i(0);i<resolution;i++)
	{
		collision = (drawLine(screen,x+sin(angle_base*(i+1))*r,y+cos(angle_base*(i+1))*r,x+sin(angle_base*(i+2))*r,y+cos(angle_base*(i+2))*r,color,w))?true:collision;
	}
	return collision;
}