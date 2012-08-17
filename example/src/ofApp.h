#pragma once

#include "ofMain.h"

#include "ofxSplineTool.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofxSplineTool splineTool;
	bool show;
};
