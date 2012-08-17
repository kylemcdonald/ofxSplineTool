#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 288, 288, OF_WINDOW);
	ofRunApp(new ofApp());
}
