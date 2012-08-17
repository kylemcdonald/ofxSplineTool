#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	splineTool.setup();
	show = true;
}  

void ofApp::update() {
}

void ofApp::draw() {
	ofBackground(128);
	ofTranslate(16, 16);
	if(show) {
		splineTool.draw();
	}
}

void ofApp::keyPressed(int key) {
	if(key == '\t') {
		show = !show;
	}
	if(key == 's') {
		splineTool.save("curves.yml");
	}
	if(key == 'l') {
		splineTool.load("curves.yml");
	}
}