#pragma once

#include "ofMain.h"

#include "Ball.h"

#include <stdio.h>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


		void loadMap(int level, int difficulty);
		void nextLevel();


		Ball ball;
		int endFlag;
		int loadFlag;
		int menuFlag;
		int loadingTime;
		int currentLevel;
		int difficulty;
		int mouseBuffer; // to prevent overclick
		ofTrueTypeFont fontBig;
		ofTrueTypeFont fontNormal;
		ofTrueTypeFont fontMini;
		
};

