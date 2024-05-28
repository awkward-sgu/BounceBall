#include "Ball.h"

Ball::Ball(){
	x = 0; y = 0;
	t = 0;
	lastY = y;
	size = 0.2;
	spawnX = 0; spawnY = 0;
	
}

void Ball::reset() {
	x = spawnX + 0.5; y = spawnY + 0.5;
	stopJumping();
}


void Ball::setSpawn(int x, int y) {
	spawnX = x;
	spawnY = y;
}

void Ball::setT() {
	t += ofGetFrameRate() / 1800;
}

void Ball::setY() {
	y = lastY + 8.8 * t - 5 * t * t; // gravity
}

void Ball::jump() {
	t = 0;
	lastY = y;
}

void Ball::stopJumping() {
	t = 1;
	lastY = y - 4.4 * 4.4 / 5;
}

void Ball::goLeft() {
	x -= ofGetFrameRate() / 1200;
}

void Ball::goRight() {
	x += ofGetFrameRate() / 1200;
}

void Ball::bounceLeft() {
	x -= ofGetFrameRate() / 1200;
}

void Ball::bounceRight() {
	x += ofGetFrameRate() / 1200;
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



void addToMap(int x, int y, blocktype blocktype) {
	Map[blockcount++] = { x, y, blocktype };
}

void cleanMap() {
	blockcount = 0;
}


