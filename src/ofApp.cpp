#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	// set window
	ofSetFrameRate(600);
	ofSetWindowTitle("Bounce Ball");
	ofSetWindowPosition(100, 100);
	ofSetWindowShape(1600, 900);


	// Window screen size
	xScale = ofGetWindowWidth() / 1600.0f;
	yScale = ofGetWindowHeight() / 900.0f;

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

	// init objects
	initMap();
	ball.reset();
	initBallMove();
}

//--------------------------------------------------------------
void ofApp::update() {

	// Window screen size
	if (ofGetWindowWidth() / 1600.0f != xScale || ofGetWindowHeight() / 900.0f != yScale) {
		xScale = ofGetWindowWidth() / 1600.0f;
		yScale = ofGetWindowHeight() / 900.0f;
		ofSetLineWidth(1 * xScale);

		// load font
		fontBig.load("CascadiaMono.ttf", 75 * xScale, true, true, true);
		fontNormal.load("CascadiaMono.ttf", 30 * xScale, true, true, true);
		fontMini.load("CascadiaMono.ttf", 20 * xScale, true, true, true);
	}



	if (menuFlag == 1) { // first menu screen
		if (loadingTime > 0) { // loading
			if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer) {
				loadingTime = 0;
			}
			else {
				loadingTime--;
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

						endFlag = 0;
						loadFlag = 1;
						currentLevel = 0;
						difficulty = 0;
					}
					else if (500 * yScale <= ofGetMouseY() && ofGetMouseY() <= 600 * yScale) { // random
						menuFlag = 2;

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
	else if (menuFlag == 2) { // second menu screen : choose difficulty
		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer) {
			if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale) {

				if (300 * yScale <= ofGetMouseY() && ofGetMouseY() <= 400 * yScale) { // easy
					menuFlag = 0;

					endFlag = 0;
					loadFlag = 1;
					difficulty = 1;
				}
				else if (500 * yScale <= ofGetMouseY() && ofGetMouseY() <= 600 * yScale) { // medium
					menuFlag = 0;

					endFlag = 0;
					loadFlag = 1;
					difficulty = 2;
				}
				else if (700 * yScale <= ofGetMouseY() && ofGetMouseY() <= 800 * yScale) { // hard
					menuFlag = 0;

					endFlag = 0;
					loadFlag = 1;
					difficulty = 3;
				}

			}
		}
	}
	else { // main game

		if (loadFlag) { // load map
			loadMap(currentLevel, difficulty);
			loadFlag = 0;
		}

		ball.setT(); // set current time
		ball.setY(); // set y coordinate based on time


		float rx = ball.getRealX() + ball.getRealSize(); // right x
		float lx = ball.getRealX() - ball.getRealSize(); // left x
		float uy = ball.getRealY() + ball.getRealSize(); // up y
		float dy = ball.getRealY() + ball.getRealSize(); // down y


		int jumpFlag = 0;
		int stopJumpingFlag = 0;


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

				if (curr->x < rx && ball.getRealX() < curr->x) { // block's left wall
					if (curr->y <= ball.getRealY() && ball.getRealY() <= curr->y + 1) {
						ball.bounceLeft();
					}
				}
				else if (lx < curr->x + 1 && curr->x + 1 < ball.getRealX()) { // block's right wall
					if (curr->y <= ball.getRealY() && ball.getRealY() <= curr->y + 1) {
						ball.bounceRight();
					}
				}
				// block's up & down
				else if (curr->x <= ball.getRealX() && ball.getRealX() <= curr->x + 1) {
					if (!jumpFlag && dy <= curr->y + 1 + 2 * ball.getRealSize() && uy > curr->y + 1 - 2 * ball.getRealSize()) {
						// jump when the ball touched a block on the top
						// fall gravity problem : enough space
						jumpFlag = 1;
						if (curr->blocktype == finish) {
							endFlag = 1;
						}
					}
					if (!stopJumpingFlag && uy >= curr->y && dy < curr->y + 2 * ball.getRealSize()) {
						// stop jumping when the ball touched a block on the bottom
						stopJumpingFlag = 1;
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




		if (endFlag) {
			if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer && 0 <= ofGetMouseX() && ofGetMouseX() <= MAX_X) {
				// when touched clear screen
				nextLevel();
			}
		}
		else {

			if (ball.getRealY() < 0) { // when ball touched bottom : respawn
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
				}

			}
		}

	}


	// mouseBuffer control
	if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
		mouseBuffer = 5;
	}
	if (mouseBuffer > 0) {
		mouseBuffer--;
	}



	// force stop
	if (ofGetKeyPressed('q')) {
		ofExit(1);
	}

}

//--------------------------------------------------------------
void ofApp::draw() {
	if (menuFlag == 1) { // menu
		if (loadingTime > 0) { // loading
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
		else {
			char string1[20] = "Play";
			char string2[20] = "Tutorial";
			char string3[20] = "Random";
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


			// random
			ofSetColor(ofColor::royalBlue);
			ofDrawRectRounded(400 * xScale, 500 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded(400 * xScale, 500 * yScale, 800 * xScale, 100 * yScale, 15 * xScale);
			ofFill();
			ofSetColor(ofColor::white);
			if (400 * xScale <= ofGetMouseX() && ofGetMouseX() <= 1200 * xScale && 500 * yScale <= ofGetMouseY() && ofGetMouseY() <= 600 * yScale)
				ofSetColor(ofColor::yellow);
			fontNormal.drawString(string3, (800 - 30 * 3) * xScale, 560 * yScale);


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
	}
	else if (menuFlag == 2) { // choose difficulty
		char string[30] = "Choose Difficulty";
		char string1[10] = "EASY";
		char string2[10] = "MEDIUM";
		char string3[10] = "HARD";


		// choose difficulty
		ofSetColor(ofColor::black);
		fontNormal.drawString(string, (800 - 30 * 7.5) * xScale, 160 * yScale);


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
	else {
		char levelString[20];
		if (currentLevel > 0) {
			sprintf(levelString, "Level %d", currentLevel);
		}
		else if (currentLevel == 0) {
			sprintf(levelString, "Tutorial", currentLevel);
		}
		else {
			sprintf(levelString, "Random", currentLevel);
		}
		ofSetColor(ofColor::black);
		fontNormal.drawString(levelString, 100 * xScale, 100 * yScale);


		if (currentLevel == 0) {
			char s1[10] = "Key: ";
			char s2[20] = "Finish Block: ";

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
		}


		// ball
		ofSetColor(ofColor::yellow);
		ofDrawEllipse(ball.getX() * xScale, (MAX_Y - ball.getY()) * yScale, (ball.getSize() * 2) * xScale, (ball.getSize() * 2) * yScale);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawEllipse(ball.getX() * xScale, (MAX_Y - ball.getY()) * yScale, (ball.getSize() * 2) * xScale, (ball.getSize() * 2) * yScale);
		ofNoFill();
		ofFill();



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
		float a = 2.7; // LineWidth
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



	// screen line
	ofSetColor(ofColor::black);
	ofDrawLine(0 * xScale, 900 * yScale, 1600 * xScale, 900 * yScale);
	ofDrawLine(1600 * xScale, 0 * yScale, 1600 * xScale, 900 * yScale);
}



//--------------------------------------------------------------
void ofApp::loadMap(int level, int difficulty) {
	cleanMap();

	if (level > 0) { // play

		if (difficulty == 1) { // easy
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
		else if (difficulty == 2) { // normal
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
		else if (difficulty == 3) { // hard
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

	}
	else if (level == 0) { // tutorial
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
	else { // random
		ball.setSpawn(0, 2);

		// finish  -  x >= 15
		// x: 0~19, y: 0~13 (not 14 since ball cannot be seen)

		int x = 0;
		int y = 0;
		int tempx = 0;
		int tempy = 0;
		int r;

		if (difficulty == 1) { // easy
			while (x < 15) {
				addToMap(x, y, normal);

				while (1) {
					r = rand() % NUM_EASY;
					tempx = x + easy[r].x;
					tempy = y + easy[r].y;

					if (tempx >= 0 && tempx <= 19 && tempy >= 0 && tempy <= 13) { // if block is valid
						x = tempx;
						y = tempy;
						break;
					}
				}
			}
			// finish block at the end

			addToMap(x, y, finish);//
		}
		else if (difficulty == 2) { // medium
			while (x < 15) {
				addToMap(x, y, normal);

				while (1) {
					r = rand() % NUM_MEDIUM;
					tempx = x + medium[r].x;
					tempy = y + medium[r].y;

					if (tempx >= 0 && tempx <= 19 && tempy >= 0 && tempy <= 13) { // if block is valid
						x = tempx;
						y = tempy;
						break;
					}
				}
			}
			// finish block at the end

			addToMap(x, y, finish);//
		}
		else if (difficulty == 3) { // hard
			while (x < 15) {
				addToMap(x, y, normal);

				while (1) {
					r = rand() % NUM_HARD;
					tempx = x + hard[r].x;
					tempy = y + hard[r].y;

					if (tempx >= 0 && tempx <= 19 && tempy >= 0 && tempy <= 13) { // if block is valid
						x = tempx;
						y = tempy;
						break;
					}
				}
			}
			// finish block at the end

			addToMap(x, y, finish);//
		}

		ball.reset();
	}
}

//--------------------------------------------------------------
void ofApp::nextLevel() {
	// next level macro

	if (currentLevel > 0) {
		ball.reset();
		endFlag = 0;
		currentLevel++;
		loadFlag = 1;

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
		endFlag = 0;
		loadFlag = 1;
	}
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
