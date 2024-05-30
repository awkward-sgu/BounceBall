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


	initMap();
	ball.reset();
}

//--------------------------------------------------------------
void ofApp::update() {

	if (menuFlag) {
		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer) {
			if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200) {

				if (100 <= ofGetMouseY() && ofGetMouseY() <= 200) { // play
					menuFlag = 0;

					endFlag = 0;
					loadFlag = 1;
					currentLevel = 1;
				}
				else if (300 <= ofGetMouseY() && ofGetMouseY() <= 400) { // tutorial
					menuFlag = 0;

					endFlag = 0;
					loadFlag = 1;
					currentLevel = 0;
				}
				else if (500 <= ofGetMouseY() && ofGetMouseY() <= 600) { // random
					menuFlag = 0;

					endFlag = 0;
					loadFlag = 1;
					currentLevel = -1;
				}
				else if (700 <= ofGetMouseY() && ofGetMouseY() <= 800) { // exit
					free(Map);
					ofExit(0);
				}

			}
		}

	}
	else {

		if (loadFlag) {
			loadMap(currentLevel);
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

					if (currentLevel > 7) {
						menuFlag = 1;
					}
				}
				else if (currentLevel == 0) {
					menuFlag = 1;
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

						if (currentLevel > 7) {
							menuFlag = 1;
						}
					}
					else if (currentLevel == 0) {
						menuFlag = 1;
					}
					else {
						ball.reset();
						endFlag = 0;
						loadFlag = 1;
					}
				}
				else if (500 <= ofGetMouseY() && ofGetMouseY() <= 700) { // x : exit
					menuFlag = 1;
				}

			}
		}

	}



	if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
		mouseBuffer = 5;
	}
	if (mouseBuffer > 0) {
		mouseBuffer--;
	}




	if (ofGetKeyPressed('q')) {
		ofExit(1);
	}

}

//--------------------------------------------------------------
void ofApp::draw() {
	if (menuFlag) { // menu

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


			ofSetColor(ofColor::black);
			fontMini.drawString(s1, 8 * SCALE + 40, SCALE + 22);

			ofSetColor(ofColor::yellow);
			ofDrawRectRounded(10 * SCALE, SCALE, SCALE / 2, SCALE / 2, SCALE / 20);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded(10 * SCALE, SCALE, SCALE / 2, SCALE / 2, SCALE / 20);
			ofFill();
			ofSetColor(ofColor::black);
			ofDrawTriangle(620, 65, 620, 85, 605, 75);


			ofSetColor(ofColor::yellow);
			ofDrawRectRounded(11 * SCALE, SCALE, SCALE / 2, SCALE / 2, SCALE / 20);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded(11 * SCALE, SCALE, SCALE / 2, SCALE / 2, SCALE / 20);
			ofFill();
			ofSetColor(ofColor::black);
			ofDrawTriangle(670, 65, 670, 85, 685, 75);


			ofSetColor(ofColor::black);
			fontMini.drawString(s2, 14 * SCALE + 14, SCALE + 22);

			ofSetColor(ofColor::green);
			ofDrawRectRounded(18 * SCALE, SCALE, SCALE / 2, SCALE / 2, SCALE / 20.0f);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded(18 * SCALE, SCALE, SCALE / 2, SCALE / 2, SCALE / 20.0f);
			ofFill();
		}



		ofSetColor(ofColor::yellow);
		ofDrawCircle(ball.getX(), MAX_Y - ball.getY(), ball.getSize());
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawCircle(ball.getX(), MAX_Y - ball.getY(), ball.getSize());
		ofFill();


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
				ofSetColor(ofColor::green);
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


		if (endFlag) {
			ofSetColor(ofColor::royalBlue);
			ofDrawRectRounded(7 * SCALE, 5 * SCALE, 8 * SCALE, 3 * SCALE, SCALE);
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectRounded(7 * SCALE, 5 * SCALE, 8 * SCALE, 3 * SCALE, SCALE);
			ofFill();
			ofSetColor(ofColor::yellow);
			fontBig.drawString("Clear!", 7 * SCALE + 60, 5 * SCALE + 120);
		}



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
void ofApp::loadMap(int flag) {
	cleanMap();

	if (flag > 0) { // play

		switch (flag) {
		case 1:
			ball.setSpawn(0, 2);

			addToMap(0, 0, normal);
			addToMap(1, 0, normal);
			addToMap(3, 0, normal);
			addToMap(4, 0, normal);
			addToMap(6, 1, normal);
			addToMap(8, 2, normal);
			addToMap(10, 4, normal);
			addToMap(12, 5, normal);
			addToMap(13, 7, normal);
			addToMap(15, 8, normal);
			addToMap(17, 10, finish);

			ball.reset();
			break;
		case 2:
			ball.setSpawn(0, 2);

			addToMap(0, 0, normal);
			addToMap(2, 1, normal);
			addToMap(5, 1, normal);
			addToMap(7, 3, normal);
			addToMap(10, 4, normal);
			addToMap(13, 4, normal);
			addToMap(16, 5, finish);

			ball.reset();
			break;
		case 3:
			ball.setSpawn(0, 2);

			addToMap(0, 0, normal);
			addToMap(3, 1, normal);
			addToMap(6, 3, normal);
			addToMap(9, 4, normal);
			addToMap(12, 6, normal);
			addToMap(15, 8, finish);

			ball.reset();
			break;
		case 4:
			ball.setSpawn(0, 2);

			addToMap(0, 0, normal);
			addToMap(3, 1, normal);
			addToMap(6, 3, normal);
			addToMap(8, 6, normal);
			addToMap(12, 3, normal);
			addToMap(15, 5, finish);

			ball.reset();
			break;
		case 5:
			ball.setSpawn(0, 2);

			addToMap(0, 0, normal);
			addToMap(2, 3, normal);
			addToMap(4, 6, normal);
			addToMap(8, 3, normal);
			addToMap(11, 4, normal);
			addToMap(11, 7, normal);
			addToMap(14, 9, normal);
			addToMap(17, 11, normal);

			ball.reset();
			break;
		case 6:
			ball.setSpawn(0, 2);

			addToMap(0, 0, normal);
			addToMap(2, 3, normal);
			addToMap(6, 0, normal);
			addToMap(9, 1, normal);
			addToMap(9, 4, normal);
			addToMap(9, 7, normal);
			addToMap(9, 10, normal);
			addToMap(13, 7, normal);
			addToMap(16, 9, finish);

			ball.reset();
			break;
		case 7:
			ball.setSpawn(0, 2);

			addToMap(0, 0, normal);
			addToMap(3, 2, normal);
			addToMap(3, 5, normal);
			addToMap(7, 2, normal);
			addToMap(7, 5, normal);
			addToMap(11, 2, normal);
			addToMap(14, 4, normal);
			addToMap(14, 7, normal);
			addToMap(17, 9, finish);

			ball.reset();
			break;
		default:
			ball.setSpawn(0, 2);

			addToMap(0, 0, normal);
			addToMap(2, 0, finish);

			ball.reset();
			break;
		}

	}
	else if (flag == 0) { // tutorial
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
		// finish  -  x: 15~19, y: 6~12
		ball.setSpawn(0, 2);

		addToMap(0, 0, normal);

		int x, y;

		x = ofRandom(15, 19.99);
		y =ofRandom(6, 11.99);

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
