#include "ofMain.h"

#include "DraggablePoints.h"

class WhitePoints : public SelectablePoints {
public:
	void draw(ofEventArgs& args) {
		ofSetColor(ofColor::white);
		SelectablePoints::draw(args);
	}
};

class GrayPoints : public DraggablePoints {
public:
	void draw(ofEventArgs& args) {
		ofSetColor(ofColor::gray);
		DraggablePoints::draw(args);
	}
};

class ofApp : public ofBaseApp {
public:
	WhitePoints whitePoints;
	GrayPoints grayPoints;
	
	void setup() {
		whitePoints.setClickRadius(32);
		for(int i = 0; i < 12; i++) {
			whitePoints.add(ofVec2f(ofRandomWidth(), ofRandomHeight()));
		}
		whitePoints.enableControlEvents();
		whitePoints.enableDrawEvent();
		
		grayPoints.setClickRadius(24);
		for(int i = 0; i < 12; i++) {
			grayPoints.add(ofVec2f(ofRandomWidth(), ofRandomHeight()));
		}
		grayPoints.enableControlEvents();
		grayPoints.enableDrawEvent();
	}
	void draw() {
		ofBackground(0);
	}
};

int main() {
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
