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

	menuFlag = 1;
	mouseBuffer = 0;


	ball.reset();
}

//--------------------------------------------------------------
void ofApp::update() {

	if (menuFlag) {
		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && !mouseBuffer) {
			if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200) {

				if (200 <= ofGetMouseY() && ofGetMouseY() <= 300) { // play
					menuFlag = 0;

					endFlag = 0;
					loadFlag = 1;
					currentLevel = 1;
				}
				else if (400 <= ofGetMouseY() && ofGetMouseY() <= 500) { // random mode
					menuFlag = 0;

					endFlag = 0;
					loadFlag = 1;
					currentLevel = -1;
				}
				else if (600 <= ofGetMouseY() && ofGetMouseY() <= 700) { // exit
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


		for (int i = 0; i < blockcount; i++) {

			if (Map[i].blocktype) {

				if (Map[i].x < rx && ball.getRealX() < Map[i].x) {
					if (Map[i].y <= ball.getRealY() && ball.getRealY() <= Map[i].y + 1) {
						ball.bounceLeft();
					}
				}
				else if (lx < Map[i].x + 1 && Map[i].x + 1 < ball.getRealX()) {
					if (Map[i].y <= ball.getRealY() && ball.getRealY() <= Map[i].y + 1) {
						ball.bounceRight();
					}
				}

				else if (Map[i].x <= ball.getRealX() && ball.getRealX() <= Map[i].x + 1) {
					if (!jumpFlag && dy <= Map[i].y + 1 + 2 * ball.getRealSize() && uy > Map[i].y + 1 - 2 * ball.getRealSize()) { // fall gravity problem
						jumpFlag = 1;
						if (Map[i].blocktype == finish) {
							endFlag = 1;
						}
					}
					if (!stopJumpingFlag && uy >= Map[i].y && dy < Map[i].y + 2 * ball.getRealSize()) {
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
				ball.reset();
				endFlag = 0;
				currentLevel++;
				loadFlag = 1;

				if (currentLevel > 7) {
					menuFlag = 1;
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
					ball.reset();
					endFlag = 0;
					currentLevel++;
					loadFlag = 1;

					if (currentLevel > 7) {
						menuFlag = 1;
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
		ofExit(0);
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	if (menuFlag) {
		char string1[20] = "Play";
		char string2[20] = "Random Mode";
		char string3[20] = "Exit";

		// play
		ofSetColor(ofColor::royalBlue);
		ofDrawRectRounded(400, 200, 800, 100, 15);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawRectRounded(400, 200, 800, 100, 15);
		ofFill();
		ofSetColor(ofColor::white);
		if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200 && 200 <= ofGetMouseY() && ofGetMouseY() <= 300)
			ofSetColor(ofColor::yellow);

		fontNormal.drawString(string1, 800 - 20 * 2, 260);


		// random mode
		ofSetColor(ofColor::royalBlue);
		ofDrawRectRounded(400, 400, 800, 100, 15);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawRectRounded(400, 400, 800, 100, 15);
		ofFill();
		ofSetColor(ofColor::white);
		if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200 && 400 <= ofGetMouseY() && ofGetMouseY() <= 500)
			ofSetColor(ofColor::yellow);
		fontNormal.drawString(string2, 800 - 20 * 6, 460);


		// exit
		ofSetColor(ofColor::royalBlue);
		ofDrawRectRounded(400, 600, 800, 100, 15);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawRectRounded(400, 600, 800, 100, 15);
		ofFill();
		ofSetColor(ofColor::white);
		if (400 <= ofGetMouseX() && ofGetMouseX() <= 1200 && 600 <= ofGetMouseY() && ofGetMouseY() <= 700)
			ofSetColor(ofColor::orangeRed);
		fontNormal.drawString(string3, 800 - 20 * 2, 660);


	}
	else {
		char levelString[20];
		sprintf(levelString, "Level %d", currentLevel);
		ofSetColor(ofColor::black);
		fontNormal.drawString(levelString, 100, 100);



		ofSetColor(ofColor::yellow);
		ofDrawCircle(ball.getX(), MAX_Y - ball.getY(), ball.getSize());
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawCircle(ball.getX(), MAX_Y - ball.getY(), ball.getSize());
		ofFill();


		for (int i = 0; i < blockcount; i++) {
			switch (Map[i].blocktype) {
			case normal:
				ofSetColor(ofColor::whiteSmoke);
				ofDrawRectRounded(Map[i].x * SCALE, (MAX_MAP_Y - Map[i].y - 1) * SCALE, SCALE, SCALE, SCALE / 10.0f);
				ofNoFill();
				ofSetColor(ofColor::black);
				ofDrawRectRounded(Map[i].x * SCALE, (MAX_MAP_Y - Map[i].y - 1) * SCALE, SCALE, SCALE, SCALE / 10.0f);
				ofFill();
				break;
			case finish:
				ofSetColor(ofColor::green);
				ofDrawRectRounded(Map[i].x * SCALE, (MAX_MAP_Y - Map[i].y - 1) * SCALE, SCALE, SCALE, SCALE / 10.0f);
				ofNoFill();
				ofSetColor(ofColor::black);
				ofDrawRectRounded(Map[i].x * SCALE, (MAX_MAP_Y - Map[i].y - 1) * SCALE, SCALE, SCALE, SCALE / 10.0f);
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
			addToMap(8, 3, normal);
			addToMap(10, 5, finish);

			ball.reset();
			break;
		case 2:
			ball.setSpawn(0, 2);

			addToMap(0, 0, normal);
			addToMap(2, 1, normal);
			addToMap(5, 1, normal);
			addToMap(7, 3, normal);
			addToMap(10, 4, finish);

			ball.reset();
			break;
		case 3:
			ball.setSpawn(0, 2);

			addToMap(0, 0, normal);
			addToMap(3, 0, normal);
			addToMap(6, 1, normal);
			addToMap(9, 2, normal);
			addToMap(12, 4, finish);

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
			addToMap(14, 9, finish);

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
			addToMap(13, 7, finish);

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
	else { // random play

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
