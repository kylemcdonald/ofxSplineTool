#pragma once

#include "ofMain.h"

class ofxSplineTool {
public:
	void setup(int width = 256, int height = 256, int curveResolution = 16);
	
	int size() const;
	void insert(ofVec2f controlPoint);
	void add(ofVec2f controlPoint);
	void set(int i, ofVec2f controlPoint);
	const vector<ofVec2f>& getControlPoints() const;
	void remove(int i);
	ofVec2f get(float t);
	float getLength() const;
	ofRectangle getBoundingBox() const;
	
	ofVec2f snap(const ofVec2f& point);
	ofVec2f snapY(const ofVec2f& point);
	
	void draw(int x = 0, int y = 0);
	void save(string filename);
	void load(string filename);
	
	// these are only used internally
	ofxSplineTool();
	void mouseMoved(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	void keyPressed(ofKeyEventArgs& args);
	void keyReleased(ofKeyEventArgs& args) {}
	void drawEvent(ofEventArgs& args);
	
protected:
	void update();
	ofPath path;
	ofPolyline polyline;
	ofRectangle boundingBox;
	vector<ofVec2f> controlPoints;
	int width, height, curveResolution;
	float length;
	
	ofVec2f drawPosition;
	
	void updateMouse(ofMouseEventArgs& args);
	int mouseX, mouseY;
	bool focus;
	
	static const int minDistance = 10;
	bool hoverState;
	bool dragState;
	int curHover;
	bool drawing, drawn;
};
