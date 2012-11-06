#pragma once

#include "ofMain.h"

#include "ofxSplineTool.h"

class Snap : public ofxSplineTool {
public:
	float smoothStep(float x) {
		return 3.*(x*x)-2.*(x*x*x);
	}
	ofVec2f getSnapped(float t, float radius) {
		int nearest = roundf(t * (size() - 1)); // index of nearest control point
		float nearestT = (float) nearest / (size() - 1); // t value of nearest control point
		ofVec2f cur = get(t); // reference point
		ofVec2f& snapped = controlPoints[nearest];
		float length = snapped.distance(cur);
		if(length < radius) {
			float weight = length / radius;
			weight = smoothStep(weight);
			return snapped.getInterpolated(cur, weight);
		} else {
			return cur;
		}
	}
};

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	Snap splineTool;
};
