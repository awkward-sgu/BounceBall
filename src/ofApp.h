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


		void initAll();

		void updateScreen();
		void updateMenu1();
		void updateMenu2();
		void updateMenu3();
		void updateGame();
		void updateBounce();
		void updateBuffer();


		void drawDefaultScreen();
		void drawMenu1();
		void drawLoadingScreen();
		void drawMenu2();
		void drawMenu3();
		void drawGame();
		void drawTutorial();



		void loadMap(int level, int difficulty); // load map based on level and difficulty
		void setMapEasy(int level);
		void setMapMedium(int level);
		void setMapHard(int level);
		void setMapTutorial();
		void setMapRandom(int difficulty);
		void setMapMeteor();
		void nextLevel(); // next level macro


		Ball ball; // B.A.L.L. - main object
		bool endFlag; // set to true when game ends -> clear screen
		bool loadFlag; // load map when true
		int menuFlag; // set to 1 initially // 1. menu screen // 2. choose difficulty screen // 0. play screen
		int loadingTime; // shows loading screen when loadingTime is true
		int currentLevel;
		int difficulty;
		int mouseBuffer; // to prevent overclick
		int helpFlag; // tutorial help

		float xScale; // scale of x : based on screen size
		float yScale; // scale of y : based on screen size

		bool full; // full screen on/off
		int screenBuffer; // to prevent overclick : full screen on/off


		ofTrueTypeFont fontBig;
		ofTrueTypeFont fontNormal;
		ofTrueTypeFont fontMini;
		
};

