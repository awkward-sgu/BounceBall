#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(600);
	ofSetWindowTitle("Bounce Ball");
	ofSetWindowPosition(100, 100);
	ofSetWindowShape(1600, 900);


	ofSetBackgroundColor(ofColor::white);
	ofSetColor(ofColor::white);
	ofSetLineWidth(1);
	
	fontBig.load("CascadiaMono.ttf", 75, true, true, true);
	fontNormal.load("CascadiaMono.ttf", 30, true, true, true);
	fontMini.load("CascadiaMono.ttf", 20, true, true, true);

	menuFlag = 1;
	mouseBuffer = 0;
	loadingTime = 200;


	initMap();
	ball.reset();
	initBallMove();
}

//--------------------------------------------------------------
void ofApp::update() {

	if (menuFlag == 1) {
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
				if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200) {

					if (100 <= ofGetMouseY() && ofGetMouseY() <= 200) { // play
						menuFlag = 2;

						currentLevel = 1;
					}
					else if (300 <= ofGetMouseY() && ofGetMouseY() <= 400) { // tutorial
						menuFlag = 0;

						endFlag = 0;
						loadFlag = 1;
						currentLevel = 0;
						difficulty = 0;
					}
					else if (500 <= ofGetMouseY() && ofGetMouseY() <= 600) { // random
						menuFlag = 2;

						currentLevel = -1;
					}
					else if (700 <= ofGetMouseY() && ofGetMouseY() <= 800) { // exit
						free(Map);
						ofExit(0);
					}

				}
			}
		}

	}
	else if (menuFlag == 2) {
		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer) {
			if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200) {

				if (300 <= ofGetMouseY() && ofGetMouseY() <= 400) { // easy
					menuFlag = 0;

					endFlag = 0;
					loadFlag = 1;
					difficulty = 1;
				}
				else if (500 <= ofGetMouseY() && ofGetMouseY() <= 600) { // medium
					menuFlag = 0;

					endFlag = 0;
					loadFlag = 1;
					difficulty = 2;
				}
				else if (700 <= ofGetMouseY() && ofGetMouseY() <= 800) { // hard
					menuFlag = 0;

					endFlag = 0;
					loadFlag = 1;
					difficulty = 3;
				}

			}
		}
	}
	else {

		if (loadFlag) {
			loadMap(currentLevel, difficulty);
			loadFlag = 0;
		}

		ball.setT();
		ball.setY();


		float rx = ball.getRealX() + ball.getRealSize(); // right x
		float lx = ball.getRealX() - ball.getRealSize(); // left x
		float uy = ball.getRealY() + ball.getRealSize(); // up y
		float dy = ball.getRealY() + ball.getRealSize(); // down y


		int jumpFlag = 0;
		int stopJumpingFlag = 0;

		if (lx < 0) {
			ball.bounceRight();
		}
		else if (rx >= MAX_MAP_X) {
			ball.bounceLeft();
		}


		block* curr = Map;

		for (int i = 0; i < blockcount; i++) {
			curr = curr->next;

			if (curr->blocktype) {

				if (curr->x < rx && ball.getRealX() < curr->x) {
					if (curr->y <= ball.getRealY() && ball.getRealY() <= curr->y + 1) {
						ball.bounceLeft();
					}
				}
				else if (lx < curr->x + 1 && curr->x + 1 < ball.getRealX()) {
					if (curr->y <= ball.getRealY() && ball.getRealY() <= curr->y + 1) {
						ball.bounceRight();
					}
				}

				else if (curr->x <= ball.getRealX() && ball.getRealX() <= curr->x + 1) {
					if (!jumpFlag && dy <= curr->y + 1 + 2 * ball.getRealSize() && uy > curr->y + 1 - 2 * ball.getRealSize()) { // fall gravity problem
						jumpFlag = 1;
						if (curr->blocktype == finish) {
							endFlag = 1;
						}
					}
					if (!stopJumpingFlag && uy >= curr->y && dy < curr->y + 2 * ball.getRealSize()) {
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
		}
		else {

			if (ball.getRealY() < 0) {
				ball.reset();
			}


			if (ofGetKeyPressed(OF_KEY_LEFT)) {
				ball.goLeft();
			}
			if (ofGetKeyPressed(OF_KEY_RIGHT)) {
				ball.goRight();
			}
		}



		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer) {
			if (1300 <= ofGetMouseX() && ofGetMouseX() <= 1500) {

				if (200 <= ofGetMouseY() && ofGetMouseY() <= 400) { // >> : next level
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
				else if (500 <= ofGetMouseY() && ofGetMouseY() <= 700) { // x : exit
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
			ofDrawCircle(500, 250, 50);
			ofFill();

			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawCircle(700, 180, 50);
			ofFill();

			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawCircle(900, 250, 50);
			ofFill();

			// ball
			ofSetColor(ofColor::yellow);
			ofDrawCircle(1020, 450, 50);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawCircle(1020, 450, 50);
			ofFill();


			ofSetColor(ofColor::black);
			fontBig.drawString(string, 850 - 75 * 5.5, 650);
		}
		else {
			char string1[20] = "Play";
			char string2[20] = "Tutorial";
			char string3[20] = "Random";
			char string4[20] = "Exit";

			// play
			ofSetColor(ofColor::royalBlue);
			ofDrawRectRounded(400, 100, 800, 100, 15);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded(400, 100, 800, 100, 15);
			ofFill();
			ofSetColor(ofColor::white);
			if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200 && 100 <= ofGetMouseY() && ofGetMouseY() <= 200)
				ofSetColor(ofColor::yellow);
			fontNormal.drawString(string1, 800 - 30 * 2, 160);


			// tutorial
			ofSetColor(ofColor::royalBlue);
			ofDrawRectRounded(400, 300, 800, 100, 15);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded(400, 300, 800, 100, 15);
			ofFill();
			ofSetColor(ofColor::white);
			if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200 && 300 <= ofGetMouseY() && ofGetMouseY() <= 400)
				ofSetColor(ofColor::yellow);
			fontNormal.drawString(string2, 800 - 30 * 4, 360);


			// random
			ofSetColor(ofColor::royalBlue);
			ofDrawRectRounded(400, 500, 800, 100, 15);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded(400, 500, 800, 100, 15);
			ofFill();
			ofSetColor(ofColor::white);
			if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200 && 500 <= ofGetMouseY() && ofGetMouseY() <= 600)
				ofSetColor(ofColor::yellow);
			fontNormal.drawString(string3, 800 - 30 * 3, 560);


			// exit
			ofSetColor(ofColor::royalBlue);
			ofDrawRectRounded(400, 700, 800, 100, 15);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded(400, 700, 800, 100, 15);
			ofFill();
			ofSetColor(ofColor::white);
			if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200 && 700 <= ofGetMouseY() && ofGetMouseY() <= 800)
				ofSetColor(ofColor::orangeRed);
			fontNormal.drawString(string4, 800 - 30 * 2, 760);

		}
	}
	else if (menuFlag == 2) { // choose difficulty
		char string[30] = "Choose Difficulty";
		char string1[10] = "EASY";
		char string2[10] = "MEDIUM";
		char string3[10] = "HARD";


		// choose difficulty
		ofSetColor(ofColor::black);
		fontNormal.drawString(string, 800 - 30 * 7.5, 160);


		// easy
		ofSetColor(ofColor::green);
		ofDrawRectRounded(400, 300, 800, 100, 15);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawRectRounded(400, 300, 800, 100, 15);
		ofFill();
		ofSetColor(ofColor::black);
		if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200 && 300 <= ofGetMouseY() && ofGetMouseY() <= 400)
			ofSetColor(ofColor::darkGray);
		fontNormal.drawString(string1, 800 - 30 * 2, 360);


		// medium
		ofSetColor(ofColor::yellow);
		ofDrawRectRounded(400, 500, 800, 100, 15);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawRectRounded(400, 500, 800, 100, 15);
		ofFill();
		ofSetColor(ofColor::black);
		if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200 && 500 <= ofGetMouseY() && ofGetMouseY() <= 600)
			ofSetColor(ofColor::darkGray);
		fontNormal.drawString(string2, 800 - 30 * 3, 560);


		// hard
		ofSetColor(ofColor::orangeRed);
		ofDrawRectRounded(400, 700, 800, 100, 15);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawRectRounded(400, 700, 800, 100, 15);
		ofFill();
		ofSetColor(ofColor::black);
		if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200 && 700 <= ofGetMouseY() && ofGetMouseY() <= 800)
			ofSetColor(ofColor::darkGray);
		fontNormal.drawString(string3, 800 - 30 * 2, 760);


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
		fontNormal.drawString(levelString, 100, 100);


		if (currentLevel == 0) {
			char s1[10] = "Key: ";
			char s2[20] = "Finish Block: ";

			// Key: 
			ofSetColor(ofColor::black);
			fontMini.drawString(s1, 8 * SCALE + 40, SCALE + 22);

			// left direction key
			ofSetColor(ofColor::yellow);
			ofDrawRectRounded(10 * SCALE, SCALE, SCALE / 2, SCALE / 2, SCALE / 20);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded(10 * SCALE, SCALE, SCALE / 2, SCALE / 2, SCALE / 20);
			ofFill();
			ofSetColor(ofColor::black);
			ofDrawTriangle(620, 65, 620, 85, 605, 75);

			// right direction key
			ofSetColor(ofColor::yellow);
			ofDrawRectRounded(11 * SCALE, SCALE, SCALE / 2, SCALE / 2, SCALE / 20);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded(11 * SCALE, SCALE, SCALE / 2, SCALE / 2, SCALE / 20);
			ofFill();
			ofSetColor(ofColor::black);
			ofDrawTriangle(670, 65, 670, 85, 685, 75);

			// Finish Block: 
			ofSetColor(ofColor::black);
			fontMini.drawString(s2, 14 * SCALE + 14, SCALE + 22);

			// mini finish block
			ofSetColor(ofColor::royalBlue);
			ofDrawRectRounded(18 * SCALE, SCALE, SCALE / 2, SCALE / 2, SCALE / 20.0f);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded(18 * SCALE, SCALE, SCALE / 2, SCALE / 2, SCALE / 20.0f);
			ofFill();
		}


		// ball
		ofSetColor(ofColor::yellow);
		ofDrawCircle(ball.getX(), MAX_Y - ball.getY(), ball.getSize());
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawCircle(ball.getX(), MAX_Y - ball.getY(), ball.getSize());
		ofFill();



		// Map
		block* curr = Map;

		for (int i = 0; i < blockcount; i++) {
			curr = curr->next;

			switch (curr->blocktype) {
			case normal:
				ofSetColor(ofColor::whiteSmoke);
				ofDrawRectRounded(curr->x * SCALE, (MAX_MAP_Y - curr->y - 1) * SCALE, SCALE, SCALE, SCALE / 10.0f);
				ofNoFill();
				ofSetColor(ofColor::black);
				ofDrawRectRounded(curr->x * SCALE, (MAX_MAP_Y - curr->y - 1) * SCALE, SCALE, SCALE, SCALE / 10.0f);
				ofFill();
				break;
			case finish:
				if (difficulty == 1)
					ofSetColor(ofColor::green);
				else if (difficulty == 2)
					ofSetColor(ofColor::yellow);
				else if (difficulty == 3)
					ofSetColor(ofColor::orangeRed);
				else
					ofSetColor(ofColor::royalBlue);
				ofDrawRectRounded(curr->x * SCALE, (MAX_MAP_Y - curr->y - 1) * SCALE, SCALE, SCALE, SCALE / 10.0f);
				ofNoFill();
				ofSetColor(ofColor::black);
				ofDrawRectRounded(curr->x * SCALE, (MAX_MAP_Y - curr->y - 1) * SCALE, SCALE, SCALE, SCALE / 10.0f);
				ofFill();
				break;
			default:
				break;

			}
		}


		if (endFlag) { // clear
			ofSetColor(ofColor::royalBlue);
			ofDrawRectRounded(7 * SCALE, 5 * SCALE, 8 * SCALE, 3 * SCALE, SCALE);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded(7 * SCALE, 5 * SCALE, 8 * SCALE, 3 * SCALE, SCALE);
			ofFill();
			ofSetColor(ofColor::yellow);
			fontBig.drawString("Clear!", 7 * SCALE + 60, 5 * SCALE + 120);
		}


		// vertical line
		ofSetColor(ofColor::black);
		ofDrawLine(MAX_X, 0, MAX_X, MAX_Y);



		// >> : next level
		ofSetColor(ofColor::royalBlue);
		ofDrawRectRounded(1300, 200, 200, 200, 20);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawRectRounded(1300, 200, 200, 200, 20);
		ofFill();
		ofSetColor(ofColor::white);
		if (1300 <= ofGetMouseX() && ofGetMouseX() <= 1500 && 200 <= ofGetMouseY() && ofGetMouseY() <= 400)
			ofSetColor(ofColor::yellow);
		ofSetLineWidth(10);
		float a = 3.6; // LineWidth
		ofDrawLine(1350, 250, 1400 + a, 300 + a);
		ofDrawLine(1350, 350, 1400 + a, 300 - a);
		ofDrawLine(1400, 250, 1450 + a, 300 + a);
		ofDrawLine(1400, 350, 1450 + a, 300 - a);
		ofSetLineWidth(1);



		// x : exit
		ofSetColor(ofColor::royalBlue);
		ofDrawRectRounded(1300, 500, 200, 200, 20);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawRectRounded(1300, 500, 200, 200, 20);
		ofFill();
		ofSetColor(ofColor::white);
		if (1300 <= ofGetMouseX() && ofGetMouseX() <= 1500 && 500 <= ofGetMouseY() && ofGetMouseY() <= 700)
			ofSetColor(ofColor::orangeRed);
		ofSetLineWidth(10);
		ofDrawLine(1350, 550, 1450, 650);
		ofDrawLine(1350, 650, 1450, 550);
		ofSetLineWidth(1);
	}
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
				addToMap(13, 7, normal);
				addToMap(17, 4, finish);//
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
		addToMap(18, 8, finish);

		ball.reset();
	}
	else { // random
		// finish  -  x: 15~19, y: 0~12
		ball.setSpawn(0, 2);

		addToMap(0, 0, normal);

		int x, y;

		x = ofRandom(15, 19.99);
		y = ofRandom(6, 11.99);

		if (difficulty == 1) {

		}
		else if (difficulty == 2) {

		}
		else if (difficulty == 3) {

		}




		addToMap(18, 8, finish);

		ball.reset();
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
