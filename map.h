#ifndef MAP_H
#define MAP_H

#include "display_sdl.h"

#define MAPWIDTH	80
#define MAPHEIGHT	40

extern int pX;
extern int pY;

class CELL
{
	friend class MAP;
private:
	char tile;
	bool seen;
	bool remembered;

public:
};

class MAP
{
private:
	CELL cells[MAPWIDTH][MAPHEIGHT];

public:
	MAP(unsigned seed);

	void display(void);
	void setSeen(unsigned int x, unsigned int y);
	bool onMap(unsigned int x, unsigned int y);
	bool blockLOS(unsigned int x, unsigned int y);

};


#endif
