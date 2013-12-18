#include "ofMain.h"

bool isDirectionKey(int key) {
	return
	key == OF_KEY_LEFT ||
	key == OF_KEY_RIGHT ||
	key == OF_KEY_UP ||
	key == OF_KEY_DOWN;
}

ofVec2f getDirectionFromKey(int key) {
	switch(key) {
		case OF_KEY_LEFT: return ofVec2f(-1, 0);
		case OF_KEY_RIGHT: return ofVec2f(+1, 0);
		case OF_KEY_UP: return ofVec2f(0, -1);
		case OF_KEY_DOWN: return ofVec2f(0, +1);
	}
	return ofVec2f(0, 0);
}

class Draggable {
protected:
	ofVec2f position;
	
	ofVec2f mouseStart, positionStart;
	bool dragging, selected, command;
	
	virtual bool isHit(ofVec2f v) = 0;
	
public:
	Draggable()
	:dragging(false)
	,selected(false)
	,command(false) {
		
	}
	void setPosition(ofVec2f position) {
		this->position = position;
	}
	void mousePressed(ofMouseEventArgs& mouse) {
		if(mouse.button == OF_MOUSE_BUTTON_LEFT) {
			if(isHit(mouse)) {
				dragging = true;
				selected = true;
				mouseStart = mouse;
				positionStart = position;
			} else {
				dragging = false;
				selected = false;
			}
		}
	}
	void mouseMoved(ofMouseEventArgs& mouse) {
		if(dragging) {
			ofVec2f offset = mouse - mouseStart;
			position = positionStart + offset;
		}
	}
	void mouseDragged(ofMouseEventArgs& mouse) {
		mouseMoved(mouse);
	}
	void mouseReleased(ofMouseEventArgs& mouse) {
		if(mouse.button == OF_MOUSE_BUTTON_LEFT) {
			dragging = false;
		}
	}
	void keyPressed(ofKeyEventArgs& key) {
		if(selected && isDirectionKey(key.key)) {
			float multiplier = command ? .25 : 1;
			position += multiplier * getDirectionFromKey(key.key);
		}
		if(key.key == OF_KEY_COMMAND) {
			command = true;
		}
	}
	void keyReleased(ofKeyEventArgs& key) {
		if(key.key == OF_KEY_COMMAND) {
			command = false;
		}
	}
	virtual void draw(ofEventArgs& args) = 0;
	void enableControlEvents() {
		ofAddListener(ofEvents().keyPressed, this, &Draggable::keyPressed);
		ofAddListener(ofEvents().keyReleased, this, &Draggable::keyReleased);
		ofAddListener(ofEvents().mousePressed, this, &Draggable::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &Draggable::mouseReleased);
		ofAddListener(ofEvents().mouseMoved, this, &Draggable::mouseMoved);
		ofAddListener(ofEvents().mouseDragged, this, &Draggable::mouseDragged);
	}
	void disableControlEvents() {
		ofRemoveListener(ofEvents().keyPressed, this, &Draggable::keyPressed);
		ofRemoveListener(ofEvents().keyReleased, this, &Draggable::keyReleased);
		ofRemoveListener(ofEvents().mousePressed, this, &Draggable::mousePressed);
		ofRemoveListener(ofEvents().mouseReleased, this, &Draggable::mouseReleased);
		ofRemoveListener(ofEvents().mouseMoved, this, &Draggable::mouseMoved);
		ofRemoveListener(ofEvents().mouseDragged, this, &Draggable::mouseDragged);
	}
	void enableDrawEvent() {
		ofAddListener(ofEvents().draw, this, &Draggable::draw);
	}
	void disableDrawEvent() {
		ofRemoveListener(ofEvents().draw, this, &Draggable::draw);
	}
};

class DraggableCircle : public Draggable {
protected:
	float clickRadiusSquared;
	
	bool isHit(ofVec2f v) {
		float dx = position.x - v.x, dy = position.y - v.y;
		float distanceSquared = dx * dx + dy * dy;
		return distanceSquared < clickRadiusSquared;
	}
public:
	DraggableCircle()
	:clickRadiusSquared(100) {
	}
	void setClickRadius(float clickRadius = 10) {
		this->clickRadiusSquared = clickRadius * clickRadius;
	}
};

class Tag : public DraggableCircle {
public:
	void draw(ofEventArgs& args) {
		float r = sqrt(clickRadiusSquared);
		ofPushStyle();
		ofNoFill();
		ofSetLineWidth(2);
		if(selected) {
			ofSetColor(ofColor::yellow);
			ofCircle(position, r + 4);
		}
		if(dragging) {
			ofSetColor(ofColor::red);
			ofCircle(position, r + 2);
		}
		ofFill();
		ofSetColor(ofColor::white);
		ofCircle(position, r);
		ofPopStyle();
	}
};

class ofApp : public ofBaseApp {
public:
	vector<Tag> p;
	
	void setup() {
		p.resize(12);
		for(int i = 0; i < p.size(); i++) {
			p[i].setPosition(ofVec2f(ofRandomWidth(), ofRandomHeight()));
			p[i].setClickRadius(64);
			p[i].enableControlEvents();
			p[i].enableDrawEvent();
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