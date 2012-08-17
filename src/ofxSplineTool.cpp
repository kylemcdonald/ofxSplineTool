#include "ofxSplineTool.h"

GLdouble modelviewMatrix[16], projectionMatrix[16];
GLint viewport[4];
void updateProjectionState() {
	glGetDoublev(GL_MODELVIEW_MATRIX, modelviewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
}

ofVec3f worldToScreen(ofVec3f world) {
	updateProjectionState();
	GLdouble x, y, z;
	gluProject(world.x, world.y, world.z, modelviewMatrix, projectionMatrix, viewport, &x, &y, &z);
	ofVec3f screen(x, y, z);
	screen.y = ofGetHeight() - screen.y;
	return screen;
}

ofxSplineTool::ofxSplineTool()
:mouseX(0)
,mouseY(0)
,hoverState(false)
,dragState(false)
,curHover(0)
,focus(false)
,drawing(false)
,drawn(false) {
	ofAddListener(ofEvents().draw, this, &ofxSplineTool::drawEvent);
}

void ofxSplineTool::setup(int n, int curveResolution) {
	this->n = n;
	this->curveResolution = curveResolution;
}

void ofxSplineTool::add(ofVec2f controlPoint) {
	controlPoints.push_back(controlPoint);
	update();
}

void ofxSplineTool::insert(ofVec2f controlPoint) {
	if(size() < 2) {
		add(controlPoint);
	} else {
		unsigned int index;
		polyline.getClosestPoint(controlPoint, &index);
		if(index == (curveResolution * (size() - 1)) - 1) {
			add(controlPoint);
			return;
		} else if(index == 0) {
			index = -1;
		} else {
			index /= curveResolution;
		}
		controlPoints.insert(controlPoints.begin() + index + 1, controlPoint);
		update();
	}
}

void ofxSplineTool::set(int i, ofVec2f controlPoint) {
	controlPoints[i] = controlPoint;
	update();
}

void ofxSplineTool::remove(int i) {
	controlPoints.erase(controlPoints.begin() + i);
	update();
}

int ofxSplineTool::size() const {
	return controlPoints.size();
}

void ofxSplineTool::update() {
	polyline.clear();
	path.clear();
	path.setFilled(false);
	path.setCurveResolution(curveResolution);
	int m = size();
	if(m > 1) {
		path.curveTo(controlPoints.front());
		for(int i = 0; i < m; i++) {
			ofVec2f& cur = controlPoints[i];
			path.curveTo(cur);
		}
		path.curveTo(controlPoints.back());
		vector<ofPolyline>& outline = path.getOutline();
		if(!outline.empty()) {
			polyline = outline[0];
		}
	}
}

ofVec2f ofxSplineTool::snap(const ofVec2f& point) {
	return polyline.getClosestPoint(point);
}

void ofxSplineTool::draw(int x, int y) {
	drawn = true; // we've made a call to draw
	
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(x, y);
	
	drawPosition = worldToScreen(ofVec2f(0, 0));
	
	ofPushMatrix();
	ofTranslate(0, n);
	ofScale(1, -1);
	
	ofSetColor(ofColor::black);
	ofFill();
	ofRect(0, 0, n, n);
	
	// grid
	ofSetColor(50);
	for(int i = 0; i < n; i += 64) {
		ofLine(0, i, n, i);
		ofLine(i, 0, i, n);
	}
	
	// crosshairs
	ofSetColor(100);
	ofVec2f cur;
	if(hoverState || dragState) {
		cur = controlPoints[curHover];
	} else {
		unsigned int curControlPoint;
		cur = snap(ofVec2f(mouseX, mouseY));
	}
	if(focus) {
		ofLine(0, cur.y, n, cur.y);
		ofLine(cur.x, 0, cur.x, n);
	}
	
	// outline
	ofSetColor(ofColor::white);
	ofNoFill();
	ofRect(.5, .5, n - 1, n - 1);
	
	// curve
	int m = controlPoints.size();
	polyline.draw();
	
	// control points
	for(int i = 0; i < m; i++) {
		ofPushMatrix();
		ofVec2f& cur = controlPoints[i];
		ofTranslate((int) cur.x, (int) cur.y);
		if(hoverState && curHover == i) {
			if(dragState) {
				ofFill();
			} else {
				ofNoFill();
			}
			ofCircle(0, 0, 3);
		} else {
			ofFill();
			ofCircle(0, 0, 2);
		}
		ofPopMatrix();
	}
	ofPopMatrix();
	
	// info for current point
	if(focus) {
		string label = ofToString((int) cur.x) + ", " + ofToString((int) cur.y);
		ofDrawBitmapString(label, 4, 18);
	}
	ofPopMatrix();
	ofPopStyle();
}

void ofxSplineTool::save(string filename) {
	ofFile out(filename, ofFile::WriteOnly);
	out << "[";
	int m = controlPoints.size();
	for(int i = 0; i < m; i++) {
		ofVec2f& cur = controlPoints[i];
		out << "[" << (int) cur.x << "," << (int) cur.y << "]";
		if(i + 1 < m) {
			out << ",";
		}
	}
	out << "]";
}

// basic yml list-of-lists parser 
void ofxSplineTool::load(string filename) {
	if(ofFile(filename).exists()) {
		string in = ofFile(filename).readToBuffer();
		ofStringReplace(in, " ", "");
		vector<string> all = ofSplitString(in, "],[");
		controlPoints.clear();
		for(int i = 0; i < all.size(); i++) {
			ofStringReplace(all[i], "[", "");
			ofStringReplace(all[i], "]", "");
			vector<string> parts = ofSplitString(all[i], ",");
			add(ofVec2f(ofToFloat(parts[0]), ofToFloat(parts[1])));
		}
		update();
	}
}

void ofxSplineTool::updateMouse(ofMouseEventArgs& args) {
	mouseX = args.x - drawPosition.x;
	mouseY = n - (args.y - drawPosition.y);
	focus = dragState;
	if(ofRectangle(0, 0, n, n).inside(mouseX, mouseY)) {
		focus = true;
	} else {
		hoverState = false;
	}
	mouseX = ofClamp(mouseX, 0, n - 1);
	mouseY = ofClamp(mouseY, 0, n - 1);
	int m = controlPoints.size();
	if(focus && !dragState) {
		hoverState = false;
		for(int i = 0; i < m; i++) {
			ofVec2f& cur = controlPoints[i];
			if(cur.distance(snap(ofVec2f(mouseX, mouseY))) < minDistance) {
				curHover = i;
				hoverState = true;
			}
		}
	}
}

void ofxSplineTool::mouseMoved(ofMouseEventArgs& args) {
	updateMouse(args);
}

void ofxSplineTool::mousePressed(ofMouseEventArgs& args) {
	updateMouse(args);
	if(focus) {
		if(!hoverState) {
			insert(ofVec2f(mouseX, mouseY));
			updateMouse(args);
		}
		dragState = true;
		mouseDragged(args);
	}
}

void ofxSplineTool::mouseDragged(ofMouseEventArgs& args) {
	updateMouse(args);
	if(dragState) {
		set(curHover, ofVec2f(mouseX, mouseY));
	} else {
		focus = false;
	}
}

void ofxSplineTool::mouseReleased(ofMouseEventArgs& args) {
	updateMouse(args);
	dragState = false;
}

void ofxSplineTool::keyPressed(ofKeyEventArgs& args) {
	if((args.key == OF_KEY_DEL || args.key == OF_KEY_BACKSPACE) && hoverState) {
		remove(curHover);
		hoverState = false;
		dragState = false;
	}
}

/*
 this part controls whether events get through to the object or not. if the
 object is not drawn using draw(), we know here because draw() never set the
 drawn flag. in that case, we unregister events. if it changes from being off
 to on, then we register the events again.
 */
void ofxSplineTool::drawEvent(ofEventArgs& args) {
	bool prevDrawing = drawing;
	drawing = drawn;
	if(drawing != prevDrawing) {
		if(drawing) {
			ofRegisterMouseEvents(this);
			ofRegisterKeyEvents(this);
		} else {
			ofUnregisterMouseEvents(this);
			ofUnregisterKeyEvents(this);
		}
	}
	drawn = false; // turn the drawn flag off, for draw() to turn back on
}