#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	splineTool.setup(256);
	
	for(int i = 0; i < 5; i++) {
		splineTool.add(ofVec2f(ofRandomuf(), ofRandomuf()) * 256);
	}
}  

void ofApp::update() {
}

void ofApp::draw() {
	ofBackground(0);
	ofTranslate(16, 16);
	ofSetColor(255);
	splineTool.draw();
	ofNoFill();
	ofSetColor(ofColor::red);
	float spacing = ofMap(mouseX, 0, ofGetWidth(), .1, .01, true);
	ofSeedRandom(0);
	int i = 0;
	for(float t = 0; t <= 1; t += spacing) {
		ofVec2f cur = splineTool.getSnapped(t, 50);
		ofCircle(cur, 3);
	}
	ofCircle(splineTool.getSnapped(fmodf(ofGetElapsedTimef() / 10., 1.), 50), 6);
}
