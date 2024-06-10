#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	// set window
	ofSetFrameRate(60);
	ofSetWindowTitle("Bounce Ball");
	ofSetWindowPosition(100, 100);
	ofSetWindowShape(1600, 900);
	ofSetEscapeQuitsApp(false); // do not quit on escape

	initAll();
}

void ofApp::initAll() {
	// Window screen size
	xScale = ofGetWindowWidth() / 1600.0f;
	yScale = ofGetWindowHeight() / 900.0f;
	full = false;
	screenBuffer = 0;

	// basic setting
	ofSetBackgroundColor(ofColor::white);
	ofSetColor(ofColor::white);
	ofSetLineWidth(1 * xScale);

	// load font
	fontBig.load("CascadiaMono.ttf", 75 * xScale, true, true, true);
	fontNormal.load("CascadiaMono.ttf", 30 * xScale, true, true, true);
	fontMini.load("CascadiaMono.ttf", 20 * xScale, true, true, true);

	// set flag
	menuFlag = 1;
	mouseBuffer = 0;
	loadingTime = 200;
	helpFlag = 0;
	easteregg = false;

	// init objects
	initMap();
	ball.reset();
	initBallMove();
}

//--------------------------------------------------------------
void ofApp::update() {

	updateScreen();

	if (menuFlag == 1) { // first menu screen
		updateMenu1();
	}
	else if (menuFlag == 2) { // second menu screen : choose difficulty
		updateMenu2();
	}
	else if (menuFlag == 3) { // third menu screen : choose mode
		updateMenu3();
	}
	else { // main game
		updateGame();
	}
	
	updateBuffer();


	// force stop
	if (ofGetKeyPressed('q')) {
		ofExit(1);
	}

}

void ofApp::updateScreen() {

	// full screen on/off
	if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer && !screenBuffer) {
		if (1525 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1575 * xScale && 25 * yScale <= ofGetMouseY() && ofGetMouseY() <= 75 * yScale) {
			full = !full;
			ofSetFullscreen(full);
			screenBuffer = 50;
		}
	}

	// full screen off
	if (ofGetKeyPressed(OF_KEY_ESC) && full && !screenBuffer) {
		full = false;
		ofSetFullscreen(full);
		screenBuffer = 50;
	}





	// current window size
	float tempX = ofGetWindowWidth() / 1600.0f;
	float tempY = ofGetWindowHeight() / 900.0f;

	if (FIT_TO_SCREEN == 0) {
		if (tempX > tempY) {
			tempX = tempY;
		}
		else if (tempX < tempY) {
			tempY = tempX;
		}
	}

	// if changed
	if (tempX != xScale || tempY != yScale) {
		xScale = tempX;
		yScale = tempY;

		ofSetLineWidth(1 * xScale);

		// load font
		fontBig.load("CascadiaMono.ttf", 75 * xScale, true, true, true);
		fontNormal.load("CascadiaMono.ttf", 30 * xScale, true, true, true);
		fontMini.load("CascadiaMono.ttf", 20 * xScale, true, true, true);

		// we should minimize fond loading : it takes enormous time to load a font
	}
}
void ofApp::updateMenu1() {

	if (loadingTime > 0) { // loading
		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer && !screenBuffer) {
			loadingTime = 0;
		}
	}
	else {
		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer) {
			if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale) {

				if (100 * yScale <= ofGetMouseY() && ofGetMouseY() <= 200 * yScale) { // play
					menuFlag = 2;

					currentLevel = 1;
				}
				else if (300 * yScale <= ofGetMouseY() && ofGetMouseY() <= 400 * yScale) { // tutorial
					menuFlag = 0;

					endFlag = false;
					loadFlag = true;
					currentLevel = 0;
					difficulty = 0;
					helpFlag = 200;
				}
				else if (500 * yScale <= ofGetMouseY() && ofGetMouseY() <= 600 * yScale) { // mode
					menuFlag = 3;

					currentLevel = -1;
				}
				else if (700 * yScale <= ofGetMouseY() && ofGetMouseY() <= 800 * yScale) { // exit
					free(Map);
					ofExit(0);
				}

			}
		}
	}
}
void ofApp::updateMenu2() {

	if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer) {
		if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale) {

			if (300 * yScale <= ofGetMouseY() && ofGetMouseY() <= 400 * yScale) { // easy
				menuFlag = 0;

				endFlag = false;
				loadFlag = true;
				difficulty = 1;
			}
			else if (500 * yScale <= ofGetMouseY() && ofGetMouseY() <= 600 * yScale) { // medium
				menuFlag = 0;

				endFlag = false;
				loadFlag = true;
				difficulty = 2;
			}
			else if (700 * yScale <= ofGetMouseY() && ofGetMouseY() <= 800 * yScale) { // hard
				menuFlag = 0;

				endFlag = false;
				loadFlag = true;
				difficulty = 3;
			}

		}


		if (210 * xScale <= ofGetMouseX() && ofGetMouseX() <= 290 * xScale && 110 * yScale <= ofGetMouseY() && ofGetMouseY() <= 190 * yScale) {
			// go back
			if (currentLevel < 0) { // mode
				menuFlag = 3;
			}
			else {
				menuFlag = 1;
			}
		}
	}
}
void ofApp::updateMenu3() {

	if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer) {
		if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale) {
			if (300 * yScale <= ofGetMouseY() && ofGetMouseY() <= 400 * yScale) { // random
				menuFlag = 2;

				currentLevel = -1;
			}
			else if (500 * yScale <= ofGetMouseY() && ofGetMouseY() <= 600 * yScale) { // ?
				currentLevel = -2;
				menuFlag = 0;

				endFlag = false;
				loadFlag = true;
				difficulty = 0;
			}
		}


		if (210 * xScale <= ofGetMouseX() && ofGetMouseX() <= 290 * xScale && 110 * yScale <= ofGetMouseY() && ofGetMouseY() <= 190 * yScale) {
			// go back
			menuFlag = 1;
		}
	}
}
void ofApp::updateGame() {

	if (loadFlag) { // load map
		loadMap(currentLevel, difficulty);
		loadFlag = false;
	}


	updateBounce();

	if (currentLevel == -2) { // easteregg
		if ((100 + 30 * 3) <= ball.getX() && ball.getX() <= (100 + 30 * 4)) {
			if (13.25 <= ball.getRealY() && ball.getRealY() <= 14) {
				easteregg = true;
			}
		}
	}


	if (endFlag) {
		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer && 0 * xScale <= ofGetMouseX() && ofGetMouseX() <= MAX_X * xScale) {
			// when touched clear screen
			nextLevel();
		}
	}
	else {

		if (ball.getRealY() < 0) { // when ball touched bottom : respawn
			if (currentLevel == 0) { // tutorial
				helpFlag = 200;
			}
			ball.reset();
		}


		if (ofGetKeyPressed(OF_KEY_LEFT)) { // left direction key
			ball.goLeft();
		}
		if (ofGetKeyPressed(OF_KEY_RIGHT)) { // right direction key
			ball.goRight();
		}
	}



	if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer) {
		if (1350 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1450 * xScale) {
			if (250 * yScale <= ofGetMouseY() && ofGetMouseY() <= 350 * yScale) { // >> : next level
				nextLevel();
			}
			else if (550 * yScale <= ofGetMouseY() && ofGetMouseY() <= 650 * yScale) { // x : exit game
				menuFlag = 1;
				loadingTime = 50;
				easteregg = false;
			}

		}
	}
}
void ofApp::updateBounce() {

	ball.setT(); // set current time
	ball.setY(); // set y coordinate based on time

	float rx = ball.getRealX() + ball.getRealSize(); // right x
	float lx = ball.getRealX() - ball.getRealSize(); // left x
	float uy = ball.getRealY() + ball.getRealSize(); // up y
	float dy = ball.getRealY() + ball.getRealSize(); // down y


	bool jumpFlag = false;
	bool stopJumpingFlag = false;


	// end of map
	if (lx < 0) {
		ball.bounceRight();
	}
	else if (rx >= MAX_MAP_X) {
		ball.bounceLeft();
	}


	// Map - ball bounce
	block* curr = Map;

	for (int i = 0; i < blockcount; i++) {
		curr = curr->next;

		if (curr->blocktype) {
			if (curr->x < rx && lx < curr->x + 1) {
				if (ball.getRealX() < curr->x) { // block's left wall
					if (curr->y <= ball.getRealY() && ball.getRealY() <= curr->y + 1) {
						ball.bounceLeft();
					}
				}
				else if (curr->x + 1 < ball.getRealX()) { // block's right wall
					if (curr->y <= ball.getRealY() && ball.getRealY() <= curr->y + 1) {
						ball.bounceRight();
					}
				}
				// block's up & down
				else {
					if (dy <= curr->y + 1 + 2 * ball.getRealSize() && uy > curr->y + 1 - 2 * ball.getRealSize()) {
						// jump when the ball touched a block on the top
						// fall gravity problem : enough space
						jumpFlag = true;
						if (curr->blocktype == finish) {
							endFlag = true;
						}
					}
					if (uy >= curr->y && dy < curr->y + 2 * ball.getRealSize()) {
						// stop jumping when the ball touched a block on the bottom
						stopJumpingFlag = true;
					}
				}
			}

		}

	}


	if (jumpFlag) {
		ball.jump();
	}
	else if (stopJumpingFlag) {
		ball.stopJumping();
	}
}
void ofApp::updateBuffer() {

	// mouseBuffer control
	if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
		mouseBuffer = 5;
	}
	if (mouseBuffer > 0) {
		mouseBuffer--;
	}


	// screenBuffer control
	if (screenBuffer > 0) {
		screenBuffer--;
	}


	// loading time
	if (loadingTime > 0) {
		loadingTime--;
	}

	// hide help instruction
	if (helpFlag > 0) {
		helpFlag--;
	}
}



//--------------------------------------------------------------
void ofApp::draw() {

	drawDefaultScreen();

	if (menuFlag == 1) { // menu
		if (loadingTime > 0) { // loading
			drawLoadingScreen();
		}
		else {
			drawMenu1();
		}
	}
	else if (menuFlag == 2) { // choose difficulty
		drawMenu2();
	}
	else if (menuFlag == 3) { // choose mode
		drawMenu3();
	}
	else {
		drawGame();
	}
}


void ofApp::drawDefaultScreen() {

	// screen
	ofSetColor(ofColor::aqua);
	ofDrawRectRounded(1525 * xScale, 25 * yScale, 50 * xScale, 50 * yScale, 5 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(1525 * xScale, 25 * yScale, 50 * xScale, 50 * yScale, 5 * xScale);
	ofFill();
	ofSetColor(ofColor::black);
	if (1525 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1575 * xScale && 25 * yScale <= ofGetMouseY() && ofGetMouseY() <= 75 * yScale)
		ofSetColor(ofColor::darkGray);
	ofSetLineWidth(5 * xScale);
	if (full) {
		ofDrawLine((1555 - 2.5) * xScale, 45 * yScale, 1570 * xScale, 45 * yScale);
		ofDrawLine(1555 * xScale, 30 * yScale, 1555 * xScale, (45 + 2.5) * yScale);
		ofDrawLine(1530 * xScale, 55 * yScale, (1545 + 2.5) * xScale, 55 * yScale);
		ofDrawLine(1545 * xScale, (55 - 2.5) * yScale, 1545 * xScale, 70 * yScale);

		ofDrawLine(1555 * xScale, 45 * yScale, 1567.5 * xScale, 32.5 * yScale);
		ofDrawLine(1545 * xScale, 55 * yScale, 1532.5 * xScale, 67.5 * yScale);
	}
	else {
		ofDrawLine(1550 * xScale, 35 * yScale, (1565 + 2.5) * xScale, 35 * yScale);
		ofDrawLine(1565 * xScale, (35 - 2.5) * yScale, 1565 * xScale, 50 * yScale);
		ofDrawLine((1535 - 2.5) * xScale, 65 * yScale, 1550 * xScale, 65 * yScale);
		ofDrawLine(1535 * xScale, 50 * yScale, 1535 * xScale, (65 + 2.5) * yScale);

		ofDrawLine(1552.5 * xScale, 47.5 * yScale, 1565 * xScale, 35 * yScale);
		ofDrawLine(1547.5 * xScale, 52.5 * yScale, 1535 * xScale, 65 * yScale);
	}
	ofSetLineWidth(1 * xScale);



	// screen line
	ofSetColor(ofColor::black);
	//ofDrawLine(0 * xScale, 900 * yScale, 1600 * xScale, 900 * yScale);
	//ofDrawLine(1600 * xScale, 0 * yScale, 1600 * xScale, 900 * yScale);


	// modified to screen block
	ofDrawRectangle(0 * xScale, 900 * yScale, 1600 * xScale, ofGetWindowHeight() - 900 * yScale);
	ofDrawRectangle(1600 * xScale, 0 * yScale, ofGetWindowWidth() - 1600 * xScale, 900 * yScale);
}
void ofApp::drawMenu1() {

	char string1[20] = "Play";
	char string2[20] = "Tutorial";
	char string3[20] = "Mode";
	char string4[20] = "Exit";

	// play
	ofSetColor(ofColor::royalBlue);
	ofDrawRectRounded(400 * xScale, 100 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(400 * xScale, 100 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofFill();
	ofSetColor(ofColor::white);
	if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale && 100 * yScale <= ofGetMouseY() && ofGetMouseY() <= 200 * yScale)
		ofSetColor(ofColor::yellow);
	fontNormal.drawString(string1, (800 - 30 * 2) * xScale, 160 * yScale);


	// tutorial
	ofSetColor(ofColor::royalBlue);
	ofDrawRectRounded(400 * xScale, 300 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(400 * xScale, 300 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofFill();
	ofSetColor(ofColor::white);
	if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale && 300 * yScale <= ofGetMouseY() && ofGetMouseY() <= 400 * yScale)
		ofSetColor(ofColor::yellow);
	fontNormal.drawString(string2, (800 - 30 * 4) * xScale, 360 * yScale);


	// mode
	ofSetColor(ofColor::royalBlue);
	ofDrawRectRounded(400 * xScale, 500 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(400 * xScale, 500 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofFill();
	ofSetColor(ofColor::white);
	if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale && 500 * yScale <= ofGetMouseY() && ofGetMouseY() <= 600 * yScale)
		ofSetColor(ofColor::yellow);
	fontNormal.drawString(string3, (800 - 30 * 2) * xScale, 560 * yScale);


	// exit
	ofSetColor(ofColor::royalBlue);
	ofDrawRectRounded(400 * xScale, 700 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(400 * xScale, 700 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofFill();
	ofSetColor(ofColor::white);
	if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale && 700 * yScale <= ofGetMouseY() && ofGetMouseY() <= 800 * yScale)
		ofSetColor(ofColor::orangeRed);
	fontNormal.drawString(string4, (800 - 30 * 2) * xScale, 760 * yScale);
}
void ofApp::drawLoadingScreen() {

	char string[20] = "Bounce Ball";

	// shadow
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawEllipse(500 * xScale, 250 * yScale, 100 * xScale, 100 * yScale);
	ofFill();

	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawEllipse(700 * xScale, 180 * yScale, 100 * xScale, 100 * yScale);
	ofFill();

	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawEllipse(900 * xScale, 250 * yScale, 100 * xScale, 100 * yScale);
	ofFill();

	// ball
	ofSetColor(ofColor::yellow);
	ofDrawEllipse(1020 * xScale, 450 * yScale, 100 * xScale, 100 * yScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawEllipse(1020 * xScale, 450 * yScale, 100 * xScale, 100 * yScale);
	ofFill();


	ofSetColor(ofColor::black);
	fontBig.drawString(string, (850 - 75 * 5.5) * xScale, 650 * yScale);
}
void ofApp::drawMenu2() {
	char string[30] = "Choose Difficulty";
	char string1[10] = "EASY";
	char string2[10] = "MEDIUM";
	char string3[10] = "HARD";


	// choose difficulty
	ofSetColor(ofColor::black);
	fontNormal.drawString(string, (800 - 30 * 7.5) * xScale, 160 * yScale);


	// go back
	ofSetColor(ofColor::aqua);
	ofDrawRectRounded(210 * xScale, 110 * yScale, 80 * xScale, 80 * yScale, 10 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(210 * xScale, 110 * yScale, 80 * xScale, 80 * yScale, 10 * xScale);
	ofFill();
	ofSetColor(ofColor::black);
	if (210 * xScale <= ofGetMouseX() && ofGetMouseX() <= 290 * xScale && 110 * yScale <= ofGetMouseY() && ofGetMouseY() <= 190 * yScale)
		ofSetColor(ofColor::darkGray);
	ofDrawTriangle(225 * xScale, 150 * yScale, 270 * xScale, 125 * yScale, 270 * xScale, 175 * yScale);


	// easy
	ofSetColor(ofColor::green);
	ofDrawRectRounded(400 * xScale, 300 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(400 * xScale, 300 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofFill();
	ofSetColor(ofColor::black);
	if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale && 300 * yScale <= ofGetMouseY() && ofGetMouseY() <= 400 * yScale)
		ofSetColor(ofColor::darkGray);
	fontNormal.drawString(string1, (800 - 30 * 2) * xScale, 360 * yScale);


	// medium
	ofSetColor(ofColor::yellow);
	ofDrawRectRounded(400 * xScale, 500 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(400 * xScale, 500 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofFill();
	ofSetColor(ofColor::black);
	if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale && 500 * yScale <= ofGetMouseY() && ofGetMouseY() <= 600 * yScale)
		ofSetColor(ofColor::darkGray);
	fontNormal.drawString(string2, (800 - 30 * 3) * xScale, 560 * yScale);


	// hard
	ofSetColor(ofColor::orangeRed);
	ofDrawRectRounded(400 * xScale, 700 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(400 * xScale, 700 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofFill();
	ofSetColor(ofColor::black);
	if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale && 700 * yScale <= ofGetMouseY() && ofGetMouseY() <= 800 * yScale)
		ofSetColor(ofColor::darkGray);
	fontNormal.drawString(string3, (800 - 30 * 2) * xScale, 760 * yScale);
}
void ofApp::drawMenu3() {

	char string[30] = "Choose Mode";
	char string1[10] = "Random";
	char string2[10] = "Meteor";

	// choose mode
	ofSetColor(ofColor::black);
	fontNormal.drawString(string, (800 - 30 * 5.5) * xScale, 160 * yScale);


	// go back
	ofSetColor(ofColor::aqua);
	ofDrawRectRounded(210 * xScale, 110 * yScale, 80 * xScale, 80 * yScale, 10 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(210 * xScale, 110 * yScale, 80 * xScale, 80 * yScale, 10 * xScale);
	ofFill();
	ofSetColor(ofColor::black);
	if (210 * xScale <= ofGetMouseX() && ofGetMouseX() <= 290 * xScale && 110 * yScale <= ofGetMouseY() && ofGetMouseY() <= 190 * yScale)
		ofSetColor(ofColor::darkGray);
	ofDrawTriangle(225 * xScale, 150 * yScale, 270 * xScale, 125 * yScale, 270 * xScale, 175 * yScale);

	// random
	ofSetColor(ofColor::royalBlue);
	ofDrawRectRounded(400 * xScale, 300 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(400 * xScale, 300 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofFill();
	ofSetColor(ofColor::white);
	if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale && 300 * yScale <= ofGetMouseY() && ofGetMouseY() <= 400 * yScale)
		ofSetColor(ofColor::yellow);
	fontNormal.drawString(string1, (800 - 30 * 3) * xScale, 360 * yScale);


	// ?
	ofSetColor(ofColor::royalBlue);
	ofDrawRectRounded(400 * xScale, 500 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(400 * xScale, 500 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
	ofFill();
	ofSetColor(ofColor::white);
	if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale && 500 * yScale <= ofGetMouseY() && ofGetMouseY() <= 600 * yScale)
		ofSetColor(ofColor::yellow);
	fontNormal.drawString(string2, (800 - 30 * 3) * xScale, 560 * yScale);
}
void ofApp::drawGame() {

	char levelString[20];
	if (currentLevel > 0) {
		sprintf(levelString, "Level %d", currentLevel);
	}
	else if (currentLevel == 0) {
		sprintf(levelString, "Tutorial", currentLevel);
	}
	else if (currentLevel == -1) {
		sprintf(levelString, "Random", currentLevel);
	}
	else if (currentLevel == -2) {
		sprintf(levelString, "Meteor", currentLevel);
	}
	ofSetColor(ofColor::black);
	fontNormal.drawString(levelString, 100 * xScale, 100 * yScale);


	if (currentLevel == 0) { // tutorial
		drawTutorial();
	}


	// ball
	ofSetColor(ofColor::yellow);
	ofDrawEllipse(ball.getX() * xScale, (MAX_Y - ball.getY()) * yScale, (ball.getSize() * 2) * xScale, (ball.getSize() * 2) * yScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawEllipse(ball.getX() * xScale, (MAX_Y - ball.getY()) * yScale, (ball.getSize() * 2) * xScale, (ball.getSize() * 2) * yScale);
	ofNoFill();
	ofFill();


	// easteregg ball
	if (easteregg) {
		ofSetColor(ofColor::yellow);
		ofDrawEllipse((100 + 30 * 3.5) * xScale, (MAX_Y - 13.5 * SCALE) * yScale, (ball.getSize() * 2) * xScale, (ball.getSize() * 2) * yScale);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawEllipse((100 + 30 * 3.5) * xScale, (MAX_Y - 13.5 * SCALE) * yScale, (ball.getSize() * 2) * xScale, (ball.getSize() * 2) * yScale);
		ofNoFill();
		ofFill();
	}


	// Map
	block* curr = Map;

	for (int i = 0; i < blockcount; i++) {
		curr = curr->next;

		if (curr->blocktype) {
			if (curr->blocktype == normal) // normal block
				ofSetColor(ofColor::whiteSmoke);
			else if (curr->blocktype == finish) { // finish block
				if (difficulty == 1) // easy
					ofSetColor(ofColor::green);
				else if (difficulty == 2) // medium
					ofSetColor(ofColor::yellow);
				else if (difficulty == 3) // hard
					ofSetColor(ofColor::orangeRed);
				else // tutorial
					ofSetColor(ofColor::royalBlue);
			}
			ofDrawRectRounded((curr->x * SCALE) * xScale, ((MAX_MAP_Y - curr->y - 1) * SCALE) * yScale, SCALE * xScale, SCALE * yScale, (SCALE / 10.0f) * xScale);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded((curr->x * SCALE) * xScale, ((MAX_MAP_Y - curr->y - 1) * SCALE) * yScale, SCALE * xScale, SCALE * yScale, (SCALE / 10.0f) * xScale);
			ofFill();
		}
	}


	if (endFlag) { // clear
		ofSetColor(ofColor::royalBlue);
		ofDrawRectRounded((7 * SCALE) * xScale, (5 * SCALE) * yScale, (8 * SCALE) * xScale, (3 * SCALE) * yScale, SCALE * xScale);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawRectRounded((7 * SCALE) * xScale, (5 * SCALE) * yScale, (8 * SCALE) * xScale, (3 * SCALE) * yScale, SCALE * xScale);
		ofFill();
		ofSetColor(ofColor::yellow);
		fontBig.drawString("Clear!", (7 * SCALE + 60) * xScale, (5 * SCALE + 120) * yScale);
	}


	// vertical line : the right end of map
	ofSetColor(ofColor::black);
	ofDrawLine(MAX_X * xScale, 0 * yScale, MAX_X * xScale, MAX_Y * yScale);



	char s3[20] = "Next Level";
	char s4[20] = "Exit Game";

	// >> : next level
	ofSetColor(ofColor::royalBlue);
	ofDrawRectRounded(1350 * xScale, 250 * yScale, 100 * xScale, 100 * yScale, 10 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(1350 * xScale, 250 * yScale, 100 * xScale, 100 * yScale, 10 * xScale);
	ofFill();
	ofSetColor(ofColor::white);
	if (1350 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1450 * xScale && 250 * yScale <= ofGetMouseY() && ofGetMouseY() <= 350 * yScale)
		ofSetColor(ofColor::yellow);
	ofSetLineWidth(7.5 * xScale);
	float a = 2.1; // LineWidth
	ofDrawLine(1375 * xScale, 275 * yScale, (1400 + a) * xScale, (300 + a) * yScale);
	ofDrawLine(1375 * xScale, 325 * yScale, (1400 + a) * xScale, (300 - a) * yScale);
	ofDrawLine(1400 * xScale, 275 * yScale, (1425 + a) * xScale, (300 + a) * yScale);
	ofDrawLine(1400 * xScale, 325 * yScale, (1425 + a) * xScale, (300 - a) * yScale);
	ofSetLineWidth(1);
	ofSetColor(ofColor::black);
	fontMini.drawString(s3, (1420 - 20 * 5) * xScale, 400 * yScale);



	// x : exit game
	ofSetColor(ofColor::royalBlue);
	ofDrawRectRounded(1350 * xScale, 550 * yScale, 100 * xScale, 100 * yScale, 10 * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded(1350 * xScale, 550 * yScale, 100 * xScale, 100 * yScale, 10 * xScale);
	ofFill();
	ofSetColor(ofColor::white);
	if (1350 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1450 * xScale && 550 * yScale <= ofGetMouseY() && ofGetMouseY() <= 650 * yScale)
		ofSetColor(ofColor::orangeRed);
	ofSetLineWidth(7.5 * xScale);
	ofDrawLine(1375 * xScale, 575 * yScale, 1425 * xScale, 625 * yScale);
	ofDrawLine(1375 * xScale, 625 * yScale, 1425 * xScale, 575 * yScale);
	ofSetLineWidth(1 * xScale);
	ofSetColor(ofColor::black);
	fontMini.drawString(s4, (1420 - 20 * 4.5) * xScale, 700 * yScale);
}
void ofApp::drawTutorial() {

	char s1[10] = "Key: ";
	char s2[20] = "Finish Block: ";
	char sHelp1[40] = "Hold the direction key to move!";
	char sHelp2[10] = "Click!";

	// Key: 
	ofSetColor(ofColor::black);
	fontMini.drawString(s1, (8 * SCALE + 40) * xScale, (SCALE + 22) * yScale);

	// left direction key
	ofSetColor(ofColor::yellow);
	ofDrawRectRounded((10 * SCALE) * xScale, SCALE * yScale, (SCALE / 2) * xScale, (SCALE / 2) * yScale, (SCALE / 20) * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded((10 * SCALE) * xScale, SCALE * yScale, (SCALE / 2) * xScale, (SCALE / 2) * yScale, (SCALE / 20) * xScale);
	ofFill();
	ofSetColor(ofColor::black);
	ofDrawTriangle(620 * xScale, 65 * yScale, 620 * xScale, 85 * yScale, 605 * xScale, 75 * yScale);

	// right direction key
	ofSetColor(ofColor::yellow);
	ofDrawRectRounded((11 * SCALE) * xScale, SCALE * yScale, (SCALE / 2) * xScale, (SCALE / 2) * yScale, (SCALE / 20) * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded((11 * SCALE) * xScale, SCALE * yScale, (SCALE / 2) * xScale, (SCALE / 2) * yScale, (SCALE / 20) * xScale);
	ofFill();
	ofSetColor(ofColor::black);
	ofDrawTriangle(670 * xScale, 65 * yScale, 670 * xScale, 85 * yScale, 685 * xScale, 75 * yScale);

	// Finish Block: 
	ofSetColor(ofColor::black);
	fontMini.drawString(s2, (14 * SCALE + 14) * xScale, (SCALE + 22) * yScale);

	// mini finish block
	ofSetColor(ofColor::royalBlue);
	ofDrawRectRounded((18 * SCALE) * xScale, SCALE * yScale, (SCALE / 2) * xScale, (SCALE / 2) * yScale, (SCALE / 20.0f) * xScale);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectRounded((18 * SCALE) * xScale, SCALE * yScale, (SCALE / 2) * xScale, (SCALE / 2) * yScale, (SCALE / 20.0f) * xScale);
	ofFill();



	// Help 1: 
	int op = helpFlag * 5; // opacity
	ofSetColor(ofColor::black, op);
	fontMini.drawString(sHelp1, (5 * SCALE + 40) * xScale, (4 * SCALE + 22) * yScale);

	if (endFlag) {
		// Help 2: 
		ofSetColor(ofColor::black);
		fontMini.drawString(sHelp2, (7 * SCALE - 150) * xScale, (5 * SCALE - 32) * yScale);
		ofSetLineWidth(3 * xScale);
		ofDrawLine((7 * SCALE) * xScale, (5 * SCALE) * yScale, (7 * SCALE - 40) * xScale, (5 * SCALE - 32) * yScale);
		ofSetLineWidth(1 * xScale);
	}
}




void ofApp::loadMap(int level, int difficulty) {
	cleanMap();

	if (level > 0) { // play

		if (difficulty == 1) { // easy
			setMapEasy(level);
		}
		else if (difficulty == 2) { // normal
			setMapMedium(level);
		}
		else if (difficulty == 3) { // hard
			setMapHard(level);
		}

	}
	else if (level == 0) { // tutorial
		setMapTutorial();
	}
	else if (level == -1) { // random
		setMapRandom(difficulty);
	}
	else if (level == -2) { // meteor
		setMapMeteor();
	}
}
void ofApp::setMapEasy(int level) {
	ball.setSpawn(0, 2);

	switch (level) {
	case 1:
		addToMap(0, 0, normal);
		addToMap(1, 0, normal);
		addToMap(3, 0, normal);
		addToMap(4, 0, normal);
		addToMap(6, 0, normal);
		addToMap(8, 0, normal);
		addToMap(10, 0, normal);
		addToMap(12, 0, normal);
		addToMap(13, 0, normal);
		addToMap(15, 0, finish);//
		break;
	case 2:
		addToMap(0, 0, normal);
		addToMap(2, 0, normal);
		addToMap(4, 1, normal);
		addToMap(5, 1, normal);
		addToMap(7, 2, normal);
		addToMap(9, 3, normal);
		addToMap(11, 3, normal);
		addToMap(12, 3, normal);
		addToMap(14, 4, normal);
		addToMap(16, 5, finish);//
		break;
	case 3:
		addToMap(0, 0, normal);
		addToMap(2, 1, normal);
		addToMap(3, 3, normal);
		addToMap(5, 4, normal);
		addToMap(7, 5, normal);
		addToMap(9, 7, normal);
		addToMap(11, 7, normal);
		addToMap(13, 9, normal);
		addToMap(15, 11, finish);//
		break;
	case 4:
		addToMap(0, 0, normal);
		addToMap(1, 0, normal);
		addToMap(2, 3, normal);
		addToMap(3, 3, normal);
		addToMap(5, 5, normal);
		addToMap(7, 5, normal);
		addToMap(8, 5, normal);
		addToMap(11, 4, normal);
		addToMap(13, 6, normal);
		addToMap(16, 5, finish);//
		break;
	case 5:
		addToMap(0, 0, normal);
		addToMap(2, 2, normal);
		addToMap(5, 1, normal);
		addToMap(8, 1, normal);
		addToMap(10, 3, normal);
		addToMap(13, 3, normal);
		addToMap(16, 3, finish);//
		break;
	case 6:
		addToMap(0, 0, normal);
		addToMap(3, 0, normal);
		addToMap(6, 0, normal);
		addToMap(8, 2, normal);
		addToMap(9, 4, normal);
		addToMap(11, 6, normal);
		addToMap(13, 8, normal);
		addToMap(16, 8, finish);//
		break;
	case 7:
		addToMap(0, 0, normal);
		addToMap(2, 0, normal);
		addToMap(4, 1, normal);
		addToMap(6, 3, normal);
		addToMap(9, 2, normal);
		addToMap(12, 2, normal);
		addToMap(14, 4, normal);
		addToMap(17, 4, finish);//
		break;
	case 8:
		addToMap(0, 0, normal);
		addToMap(1, 0, normal);
		addToMap(4, 0, normal);
		addToMap(6, 0, normal);
		addToMap(9, 0, normal);
		addToMap(11, 0, normal);
		addToMap(13, 0, normal);
		addToMap(14, 0, normal);
		addToMap(17, 0, finish);//
		break;
	case 9:
		addToMap(0, 0, normal);
		addToMap(1, 2, normal);
		addToMap(3, 4, normal);
		addToMap(6, 3, normal);
		addToMap(9, 3, normal);
		addToMap(11, 5, normal);
		addToMap(13, 7, normal);
		addToMap(16, 7, finish);//
		break;
	case 10:
		addToMap(0, 0, normal);
		addToMap(1, 0, normal);
		addToMap(3, 2, normal);
		addToMap(5, 3, normal);
		addToMap(7, 3, normal);
		addToMap(10, 3, normal);
		addToMap(11, 5, normal);
		addToMap(13, 5, normal);
		addToMap(16, 5, finish);//
		break;
	default:
		addToMap(0, 0, normal);
		addToMap(2, 0, finish);//
		break;
	}

	ball.reset();
}
void ofApp::setMapMedium(int level) {

	ball.setSpawn(0, 2);

	switch (level) {
	case 1:
		addToMap(0, 0, normal);
		addToMap(3, 0, normal);
		addToMap(5, 2, normal);
		addToMap(6, 5, normal);
		addToMap(9, 5, normal);
		addToMap(11, 7, normal);
		addToMap(13, 9, normal);
		addToMap(16, 9, finish);//
		break;
	case 2:
		addToMap(0, 0, normal);
		addToMap(1, 3, normal);
		addToMap(3, 6, normal);
		addToMap(6, 5, normal);
		addToMap(9, 5, normal);
		addToMap(11, 7, normal);
		addToMap(14, 7, normal);
		addToMap(17, 7, finish);//
		break;
	case 3:
		addToMap(0, 0, normal);
		addToMap(2, 3, normal);
		addToMap(5, 3, normal);
		addToMap(8, 4, normal);
		addToMap(11, 3, normal);
		addToMap(14, 4, normal);
		addToMap(17, 4, finish);//
		break;
	case 4:
		addToMap(0, 0, normal);
		addToMap(3, 1, normal);
		addToMap(6, 2, normal);
		addToMap(9, 2, normal);
		addToMap(12, 2, normal);
		addToMap(15, 3, finish);//
		break;
	case 5:
		addToMap(0, 0, normal);
		addToMap(2, 2, normal);
		addToMap(4, 5, normal);
		addToMap(7, 6, normal);
		addToMap(10, 6, normal);
		addToMap(14, 1, normal);
		addToMap(17, 1, finish);//
		break;
	case 6:
		addToMap(0, 0, normal);
		addToMap(2, 3, normal);
		addToMap(4, 6, normal);
		addToMap(8, 1, normal);
		addToMap(11, 2, normal);
		addToMap(13, 5, normal);
		addToMap(17, 0, finish);//
		break;
	case 7:
		addToMap(0, 0, normal);
		addToMap(3, 1, normal);
		addToMap(5, 4, normal);
		addToMap(8, 5, normal);
		addToMap(12, 1, normal);
		addToMap(15, 2, finish);//
		break;
	case 8:
		addToMap(0, 0, normal);
		addToMap(2, 2, normal);
		addToMap(5, 3, normal);
		addToMap(7, 6, normal);
		addToMap(9, 9, normal);
		addToMap(13, 4, normal);
		addToMap(17, 0, finish);//
		break;
	case 9:
		addToMap(0, 0, normal);
		addToMap(1, 3, normal);
		addToMap(4, 4, normal);
		addToMap(8, 0, normal);
		addToMap(9, 3, normal);
		addToMap(12, 4, normal);
		addToMap(16, 0, finish);//
		break;
	case 10:
		addToMap(0, 0, normal);
		addToMap(3, 0, normal);
		addToMap(5, 3, normal);
		addToMap(8, 4, normal);
		addToMap(10, 7, normal);
		addToMap(13, 8, normal);
		addToMap(17, 4, finish);//
		break;
	default:
		addToMap(0, 0, normal);
		addToMap(2, 0, finish);
		break;
	}

	ball.reset();
}
void ofApp::setMapHard(int level) {

	ball.setSpawn(0, 2);

	switch (level) {
	case 1:
		addToMap(0, 0, normal);
		addToMap(3, 1, normal);
		addToMap(6, 2, normal);
		addToMap(9, 4, normal);
		addToMap(11, 7, normal);
		addToMap(15, 3, finish);//
		break;
	case 2:
		addToMap(0, 0, normal);
		addToMap(3, 2, normal);
		addToMap(6, 4, normal);
		addToMap(8, 7, normal);
		addToMap(11, 9, normal);
		addToMap(15, 5, finish);//
		break;
	case 3:
		addToMap(0, 0, normal);
		addToMap(2, 3, normal);
		addToMap(5, 5, normal);
		addToMap(9, 2, normal);
		addToMap(12, 4, normal);
		addToMap(14, 7, normal);
		addToMap(18, 4, finish);//
		break;
	case 4:
		addToMap(0, 0, normal);
		addToMap(3, 1, normal);
		addToMap(3, 4, normal);
		addToMap(6, 6, normal);
		addToMap(10, 3, normal);
		addToMap(13, 5, normal);
		addToMap(16, 7, finish);//
		break;
	case 5:
		addToMap(0, 0, normal);
		addToMap(2, 3, normal);
		addToMap(6, 0, normal);
		addToMap(6, 3, normal);
		addToMap(6, 6, normal);
		addToMap(9, 8, normal);
		addToMap(13, 5, normal);
		addToMap(17, 2, finish);//
		break;
	case 6:
		addToMap(0, 0, normal);
		addToMap(2, 3, normal);
		addToMap(6, 0, normal);
		addToMap(9, 2, normal);
		addToMap(9, 5, normal);
		addToMap(9, 8, normal);
		addToMap(9, 11, normal);
		addToMap(13, 8, normal);
		addToMap(16, 10, finish);//
		break;
	case 7:
		addToMap(0, 0, normal);
		addToMap(3, 2, normal);
		addToMap(3, 5, normal);
		addToMap(3, 8, normal);
		addToMap(7, 5, normal);
		addToMap(11, 2, normal);
		addToMap(11, 5, normal);
		addToMap(11, 8, normal);
		addToMap(14, 10, normal);
		addToMap(18, 7, finish);//
		break;
	case 8:
		addToMap(0, 0, normal);
		addToMap(0, 3, normal);
		addToMap(0, 6, normal);
		addToMap(0, 9, normal);
		addToMap(0, 12, normal);
		addToMap(4, 9, normal);
		addToMap(8, 6, normal);
		addToMap(12, 3, normal);
		addToMap(16, 0, finish);//
		break;
	case 9:
		addToMap(0, 0, normal);
		addToMap(0, 3, normal);
		addToMap(3, 5, normal);
		addToMap(3, 8, normal);
		addToMap(7, 5, normal);
		addToMap(7, 8, normal);
		addToMap(11, 5, normal);
		addToMap(11, 8, normal);
		addToMap(11, 11, normal);
		addToMap(15, 8, finish);//
		break;
	case 10:
		addToMap(0, 0, normal);
		addToMap(3, 2, normal);
		addToMap(3, 5, normal);
		addToMap(7, 2, normal);
		addToMap(10, 4, normal);
		addToMap(14, 1, normal);
		addToMap(14, 4, normal);
		addToMap(14, 7, normal);
		addToMap(14, 10, normal);
		addToMap(14, 13, normal);
		addToMap(18, 10, finish);//
		break;
	default:
		addToMap(0, 0, normal);
		addToMap(2, 0, finish);
		break;
	}

	ball.reset();
}
void ofApp::setMapTutorial() {

	ball.setSpawn(0, 2);

	addToMap(0, 0, normal);
	addToMap(1, 0, normal);
	addToMap(2, 0, normal);
	addToMap(3, 0, normal);
	addToMap(4, 1, normal);
	addToMap(5, 1, normal);
	addToMap(7, 1, normal);
	addToMap(8, 1, normal);
	addToMap(9, 1, normal);
	addToMap(9, 3, normal);
	addToMap(10, 1, normal);
	addToMap(11, 1, normal);
	addToMap(11, 4, normal);
	addToMap(12, 4, normal);
	addToMap(14, 5, normal);
	addToMap(16, 6, normal);
	addToMap(18, 8, finish);//

	ball.reset();
}
void ofApp::setMapRandom(int difficulty) {

	ball.setSpawn(0, 2);

	// finish  -  x >= 15
	// x: 0~19, y: 0~13 (not 14 since ball cannot be seen)

	int x = 0;
	int y = 0;
	int tempx = 0;
	int tempy = 0;
	int r;


	while (x < 15) {
		addToMap(x, y, normal);

		while (1) {
			if (difficulty == 1) { // easy
				r = rand() % NUM_EASY;
				tempx = x + easy[r].x;
				tempy = y + easy[r].y;
			}
			else if (difficulty == 2) { // medium
				r = rand() % NUM_MEDIUM;
				tempx = x + medium[r].x;
				tempy = y + medium[r].y;
			}
			else if (difficulty == 3) { // hard
				r = rand() % NUM_HARD;
				tempx = x + hard[r].x;
				tempy = y + hard[r].y;
			}



			if (tempx >= 0 && tempx <= 19 && tempy >= 0 && tempy <= 13) { // if block is valid
				x = tempx;
				y = tempy;
				break;
			}
		}
	}

	// finish block at the end

	addToMap(x, y, finish);//

	ball.reset();
}
void ofApp::setMapMeteor() {

	int finishX = rand() % 4 + 15; // 15 ~ 18
	int finishY = rand() % 4 + 9; // 9 ~ 12

	int* temp = (int*)malloc(sizeof(int) * finishX); // Y coordinate
	temp[0] = 0;


	bool** connected = (bool**)malloc(sizeof(bool*) * finishX); // whether normal blocks are connected
	for (int i = 0; i < finishX; i++) {
		connected[i] = (bool*)malloc(sizeof(bool) * finishX);
	}

	bool* end = (bool*)malloc(sizeof(bool*) * finishX); // wheter can end



	bool check = false;

	while (1) {

		cleanMap();



		for (int i = 1; i < finishX; i++) { // randomly set block
			temp[i] = rand() % 13; // 0 ~ 12
			if (rand() % 5) { // 80%
				addToMap(i, temp[i], normal);
			}
			else {
				temp[i] = INF;
			}
		}

		// initialize
		for (int i = 0; i < finishX; i++) {
			end[i] = false;
			for (int j = 0; j < finishX; j++) {
				connected[i][j] = false;
			}
		}



		int count;
		for (int i = 0; i < finishX; i++) {
			// check front 4 block
			for (count = 1; count <= 4; count++) {

				if (count == 1) { // count == 1
					if (i + count >= finishX) {
						if (finishY <= temp[i] + 3) {
							end[i] = true;
							break;
						}
					}
					else {
						if (temp[i + count] <= temp[i] + 3) {
							connected[i][i + count] = true;
						}
						else if (temp[i + count] <= temp[i] + 5) { // blocking path
							break;
						}
					}
				}
				else if (count == 2) { // count == 2
					if (i + count >= finishX) {
						if (finishY <= temp[i] + 3) {
							end[i] = true;
							break;
						}
					}
					else {
						if (temp[i + count] <= temp[i] + 3) {
							connected[i][i + count] = true;
						}
						else if (temp[i + count] <= temp[i] + 5) { // blocking path
							break;
						}
					}
				}
				else if (count == 3) { // count == 3
					if (i + count >= finishX) {
						if (finishY <= temp[i] + 2) {
							end[i] = true;
							break;
						}
					}
					else {
						if (temp[i + count] <= temp[i] + 2) {
							connected[i][i + count] = true;
						}
						else if (temp[i + count] <= temp[i] + 4) { // blocking path
							break;
						}
					}
				}
				else if (count == 4) { // count == 4
					if (i + count >= finishX) {
						if (finishY <= temp[i] - 3) {
							end[i] = true;
							break;
						}
					}
					else {
						if (temp[i + count] <= temp[i] - 3) {
							connected[i][i + count] = true;
						}
						else if (temp[i + count] <= temp[i] - 1) { // blocking path
							break;
						}
					}
				}
			}
		}
		



		// Floyd-Warshall
		for (int i = 0; i < finishX; i++) {
			for (int j = 0; j < finishX; j++) {
				for (int k = 0; k < finishX; k++) {
					if (connected[i][k] && connected[k][j]) {
						connected[i][j] = true;
					}
				}
			}
		}



		for (int i = 0; i < finishX; i++) { // whether there are path
			if (connected[0][i] && end[i]) {
				check = true;
			}
		}
		if (check) { // the path exists : end
			break;
		}
	}

	// free
	free(temp);
	for (int i = 0; i < finishX; i++) {
		free(connected[i]);
	}
	free(connected);
	free(end);



	ball.setSpawn(0, 2);

	addToMap(0, 0, normal);
	addToMap(finishX, finishY, finish);//

	ball.reset();
}



void ofApp::nextLevel() {
	// next level macro

	if (currentLevel > 0) {
		ball.reset();
		endFlag = false;
		currentLevel++;
		loadFlag = true;

		if (currentLevel > MAX_LEVEL) {
			menuFlag = 1;
			loadingTime = 50;
		}
	}
	else if (currentLevel == 0) {
		menuFlag = 1;
		loadingTime = 50;
	}
	else {
		ball.reset();
		endFlag = false;
		loadFlag = true;
	}

	easteregg = false;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
