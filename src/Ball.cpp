#include "Ball.h"

Ball::Ball(){
	x = 0; y = 0;
	t = 0;
	lastY = round(y) + size;
	size = 0.2;
	spawnX = 0; spawnY = 0;
	
}

void Ball::reset() {
	x = spawnX + 0.5; y = spawnY + 0.5; // + 0.5 : centering the place
	stopJumping();
}


void Ball::setSpawn(int x, int y) {
	spawnX = x;
	spawnY = y;
}

void Ball::setT() {
	t += 2 / ofGetTargetFrameRate();
}

void Ball::setY() {
	y = lastY + 8.8 * t - 5 * t * t; // gravity
}

void Ball::jump() {
	t = 0;
	lastY = y;
	if (y > round(y)) { // superjump ban
		lastY = round(y);
	}

}

void Ball::stopJumping() { // make ball stop jumping
	t = 1;
	lastY = y - 4.4 * 4.4 / 5;
	// max height & ball size (t = 4.4/5 -> t = 1)
}

void Ball::goLeft() {
	x -= 3 / ofGetTargetFrameRate();
}

void Ball::goRight() {
	x += 3 / ofGetTargetFrameRate();
}

void Ball::bounceLeft() {
	x -= 4 / ofGetTargetFrameRate();
}

void Ball::bounceRight() {
	x += 4 / ofGetTargetFrameRate();
}



float Ball::getX() {
	return SCALE * x;
}
float Ball::getY() {
	return SCALE * y;
}
float Ball::getRealX() {
	return x;
}
float Ball::getRealY() {
	return y;
}


float Ball::getSize() {
	return SCALE * size;
}
float Ball::getRealSize() {
	return size;
}



void initMap() { // make empty head node
	Map = (block*)malloc(sizeof(block));
	Map->x = -1;
	Map->y = -1;
	Map->blocktype = emptyblock;
	Map->next = NULL;

	blockcount = 0;
}

void addToMap(int x, int y, blocktype blocktype) { // linking
	block* curr = Map;
	for (int i = 0; i < blockcount; i++) {
		curr = curr->next;
	}
	curr->next = (block*)malloc(sizeof(block));
	curr = curr->next;
	curr->x = x;
	curr->y = y;
	curr->blocktype = blocktype;
	curr->next = NULL;

	blockcount++;
}

void cleanMap() { // free linkedlist except for the head node
	block* curr = Map->next;
	block* temp;
	for (int i = 0; i < blockcount; i++) {
		temp = curr;
		curr = curr->next;
		free(temp);
	}

	blockcount = 0;
}





void initBallMove() { // set possible move patterns
	easy[0] = { 1,0 };
	easy[1] = { 1,2 };
	easy[2] = { 2,0 };
	easy[3] = { 2,1 };
	easy[4] = { 2,2 };
	easy[5] = { 3,-1 };
	easy[6] = { 3,0 };

	medium[0] = { 1,3 };
	medium[1] = { 2,2 };
	medium[2] = { 2,3 };
	medium[3] = { 3,-1 };
	medium[4] = { 3,0 };
	medium[5] = { 3,1 };
	medium[6] = { 4,-5 };
	medium[7] = { 4,-4 };

	hard[0] = { 0,3 };
	hard[1] = { 2,3 };
	hard[2] = { 3,1 };
	hard[3] = { 3,2 };
	hard[4] = { 4,-4 };
	hard[5] = { 4,-3 };

}



