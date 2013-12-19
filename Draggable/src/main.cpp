#include "ofMain.h"

#include "DraggablePoints.h"

class ofApp : public ofBaseApp {
public:
	SelectablePoints points;
	
	void setup() {
		points.enableControlEvents();
		points.enableDrawEvent();
		points.setClickRadius(64);
		for(int i = 0; i < 12; i++) {
			points.add(ofVec2f(ofRandomWidth(), ofRandomHeight()));
		}
	}
	void update() {
		
	}
	void draw() {
		ofBackground(0);
	}
};

int main() {
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}