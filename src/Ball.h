
#ifndef Ball_h
#define Ball_h

#include "ofMain.h"


/* mode: choose 0 or 1*/
#define FIT_TO_SCREEN 0




// screen : 1024, 768 -> 1600, 900

// (1200, 900) = (20,15) * 60
#define MAX_MAP_X 20 // max x coordinate
#define MAX_MAP_Y 15 // max y coordinate

#define SCALE 60 // scale of a block

#define MAX_X MAX_MAP_X * SCALE
#define MAX_Y MAX_MAP_Y * SCALE

#define MAX_LEVEL 10

#define NUM_EASY 7
#define NUM_MEDIUM 8
#define NUM_HARD 6



class Ball {
private:
	float t; // time after bounce
	float lastY; // y when bounced
	float x, y; // current ball's xy coordinate
	float size; // ball's size
	short int spawnX, spawnY; // spawnpoint xy coordinate

public:
	Ball();
	void reset(); // respawn and initialize ball

	void setSpawn(short int x, short int y); // set spawnpoint
	void setT(); // set t
	void setY(); // set y based on t
	void jump(); // make ball jump
	void stopJumping(); // make ball stop jumping : when ball touched ceiling
	void goLeft(); // make ball go left : user
	void goRight(); // make ball go right : user
	void bounceLeft(); // make ball go left : when blocked by wall
	void bounceRight(); // make ball go right : when blocked by wall

	float getX(); // get x * SCALE 
	float getY(); // get y * SCALE
	float getRealX();// get x coordinate
	float getRealY(); // get y coordinate
	float getSize(); // get size * SCALE
	float getRealSize(); // get size

};

enum blocktype {
	emptyblock, normal, finish
};

// block coordinate
typedef struct _block {
	short int x, y;
	blocktype blocktype;
	struct _block* next;
}block;

block* Map; // Map info : linkedlist
int blockcount = 0; // num of blocks in the Map


void initMap(); // initialize Map
void addToMap(short int x, short int y, blocktype blocktype); // add block to Map
void cleanMap(); // make Map empty : reinitializing



// direction ball can move
typedef struct {
	short int x, y;
}ballMove;

ballMove easy[NUM_EASY]; // easy pattern
ballMove medium[NUM_MEDIUM]; // medium pattern
ballMove hard[NUM_HARD]; // hard pattern

void initBallMove(); // possible move patterns


#endif