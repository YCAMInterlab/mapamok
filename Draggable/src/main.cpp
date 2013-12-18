#include "ofMain.h"

class Draggable {
protected:
	ofVec2f position;
	
	ofVec2f mouseStart, positionStart;
	bool dragging;
	
	virtual bool isHit(float x, float y) = 0;
	
public:
	Draggable()
	:dragging(false) {
		
	}
	void setPosition(ofVec2f position) {
		this->position = position;
	}
	void mousePressed(float x, float y, int b) {
		if(isHit(x, y)) {
			dragging = true;
			mouseStart.set(x, y);
			positionStart = position;
		}
	}
	void mouseMoved(float x, float y) {
		if(dragging) {
			ofVec2f mouse(x, y);
			ofVec2f offset = mouse - mouseStart;
			position = positionStart + offset;
		}
	}
	void mouseDragged(float x, float y, int b) {
		mouseMoved(x, y);
	}
	void mouseReleased() {
		dragging = false;
	}
};

class DraggableCircle : public Draggable {
protected:
	float clickRadiusSquared;
	
	bool isHit(float x, float y) {
		float dx = position.x - x, dy = position.y - y;
		float distanceSquared = dx * dx + dy * dy;
		return distanceSquared < clickRadiusSquared;
	}
public:
	void setClickRadius(float clickRadius = 10) {
		this->clickRadiusSquared = clickRadius * clickRadius;
	}
};

class Tag : public DraggableCircle {
public:
	void draw() {
		ofCircle(position, 10);
	}
};

class ofApp : public ofBaseApp {
public:
	Tag p;
	
	void setup() {
		p.setPosition(ofVec2f(200, 200));
		p.setClickRadius(10);
	}
	void update() {

	}
	void draw() {
		p.draw();
	}
	void mousePressed(int x, int y, int b) {
		p.mousePressed(x, y, b);
	}
	void mouseReleased(int x, int y, int b) {
		p.mouseReleased();
	}
	void mouseMoved(int x, int y) {
		p.mouseMoved(x, y);
	}
	void mouseDragged(int x, int y, int b) {
		p.mouseDragged(x, y, b);
	}
};

int main() {
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}