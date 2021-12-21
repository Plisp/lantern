#include <cmath>
#include <cstdlib>

#include "map.h"

int pX = 20;
int pY = 20;

MAP::MAP(unsigned seed)
{
	srand(seed);

	// fill the map with blocking cells and set all cells to not seen
	for(int x=0; x<MAPWIDTH; x++)
	{
		for(int y=0; y<MAPHEIGHT; y++)
		{
			this->cells[x][y].tile='.';
			this->cells[x][y].seen=false;
		}
	}

	// generate a game of life like cave 
	for(int fill=0; fill<(MAPWIDTH*MAPHEIGHT*0.55); fill++)
	{
		this->cells[(int)(((float)rand()/RAND_MAX) * MAPWIDTH)][(int)(((float)rand()/RAND_MAX) * MAPHEIGHT)].tile='#';
	}
	int count=0;
	for(int apply=0; apply<1; apply++)
	{
		for(int x=0; x<MAPWIDTH; x++)
		{
			for(int y=0; y<MAPHEIGHT; y++)
			{
				count=0;
				if(this->blockLOS(x-1,y-1)) count++;	// NW
				if(this->blockLOS(x,y-1)) count++;		// N
				if(this->blockLOS(x+1,y-1)) count++;	// NE
				if(this->blockLOS(x+1,y)) count++;		// E
				if(this->blockLOS(x+1,y+1)) count++;	// SE
				if(this->blockLOS(x,y+1)) count++;		// S
				if(this->blockLOS(x-1,y+1)) count++;	// SW
				if(this->blockLOS(x-1,y)) count++;		// W
				
				// a wall is removed if there are less than four adjacent walls
				if(this->blockLOS(x,y))
				{
					if(count<4) this->cells[x][y].tile='.';
				}
				// a wall is created if there more than four adjacent walls
				else
				{
					if(count>4) this->cells[x][y].tile='#';
				}
				
			}
		}
	}

	for(int y=0; y<20; y++)
		this->cells[y][20].tile='#';
}

bool MAP::onMap(unsigned int x, unsigned int y)
{
	return (x<MAPWIDTH && y<MAPHEIGHT);
}

bool MAP::blockLOS(unsigned int x, unsigned int y)
{
	if(!onMap(x,y)) return true;

	return (this->cells[x][y].tile == '#');
}

void MAP::setSeen(unsigned int x, unsigned int y)
{
	if(!onMap(x,y)) return;

	this->cells[x][y].seen=true;
	this->cells[x][y].remembered=true;
}

void MAP::display(void)
{
	for(int x=0; x<MAPWIDTH; x++)
	{
		for(int y=0; y<MAPHEIGHT; y++)
		{
			if (this->cells[x][y].seen) {
				int dx = x - pX;
				int dy = y - pY;
				int c = 0xFF * 1/fmax(0.5, sqrt(dx*dx + dy*dy));
				c = fmax(0xF, c);
				display_put_char(this->cells[x][y].tile, x, y, c, c, c);
			} else if (this->cells[x][y].remembered)
				display_put_char(this->cells[x][y].tile, x, y, 0xF, 0xF, 0xF);
			//else
			//display_put_char(this->cells[x][y].tile, x, y, 0xFF/4, 0xFF/4, 0xFF/4);
			this->cells[x][y].seen=false;
		}
	}
}
