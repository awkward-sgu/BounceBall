
#ifndef Ball_h
#define Ball_h

#include "ofMain.h"

// screen : 1024, 768 -> 1600, 900

// (1200, 900) = (20,15) * 60
#define MAX_MAP_X 20
#define MAX_MAP_Y 15

#define SCALE 60

#define MAX_X MAX_MAP_X * SCALE
#define MAX_Y MAX_MAP_Y * SCALE



class Ball {
private:
	float t; // time after bounce
	float lastY; // y when bounced
	float x, y;
	float size;
	int spawnX, spawnY;

public:
	Ball();
	void reset();

	void setSpawn(int x, int y);
	void setT();
	void setY();
	void jump();
	void stopJumping();
	void goLeft();
	void goRight();
	void bounceLeft();
	void bounceRight();

	float getX();
	float getY();
	float getRealX();
	float getRealY();
	float getSize();
	float getRealSize();

};

enum blocktype {
	emptyblock, normal, finish
};

typedef struct {
	int x, y;
	blocktype blocktype;
}block;

block Map[MAX_MAP_X * MAX_MAP_Y];
int blockcount = 0;

void addToMap(int x, int y, blocktype blocktype);
void cleanMap();




#endif