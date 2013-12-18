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

class EventWatcher {
public:
	virtual void mousePressed(ofMouseEventArgs& mouse) {}
	virtual void mouseMoved(ofMouseEventArgs& mouse) {}
	virtual void mouseDragged(ofMouseEventArgs& mouse) {}
	virtual void mouseReleased(ofMouseEventArgs& mouse) {}
	virtual void keyPressed(ofKeyEventArgs& key) {}
	virtual void keyReleased(ofKeyEventArgs& key) {}
	virtual void draw(ofEventArgs& args) {}
	void enableControlEvents() {
		ofAddListener(ofEvents().keyPressed, this, &EventWatcher::keyPressed);
		ofAddListener(ofEvents().keyReleased, this, &EventWatcher::keyReleased);
		ofAddListener(ofEvents().mousePressed, this, &EventWatcher::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &EventWatcher::mouseReleased);
		ofAddListener(ofEvents().mouseMoved, this, &EventWatcher::mouseMoved);
		ofAddListener(ofEvents().mouseDragged, this, &EventWatcher::mouseDragged);
	}
	void disableControlEvents() {
		ofRemoveListener(ofEvents().keyPressed, this, &EventWatcher::keyPressed);
		ofRemoveListener(ofEvents().keyReleased, this, &EventWatcher::keyReleased);
		ofRemoveListener(ofEvents().mousePressed, this, &EventWatcher::mousePressed);
		ofRemoveListener(ofEvents().mouseReleased, this, &EventWatcher::mouseReleased);
		ofRemoveListener(ofEvents().mouseMoved, this, &EventWatcher::mouseMoved);
		ofRemoveListener(ofEvents().mouseDragged, this, &EventWatcher::mouseDragged);
	}
	void enableDrawEvent() {
		ofAddListener(ofEvents().draw, this, &EventWatcher::draw);
	}
	void disableDrawEvent() {
		ofRemoveListener(ofEvents().draw, this, &EventWatcher::draw);
	}
};

class DraggablePoint {
public:
	ofVec2f position, positionStart;
	bool selected, dragging;
	
	DraggablePoint()
	:selected(false)
	,dragging(false) {
	}
	bool isHit(ofVec2f v, float clickRadiusSquared) {
		return position.distanceSquared(v) < clickRadiusSquared;
	}
	void draw(float clickRadiusSquared) {
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

class DraggablePoints : public EventWatcher {
protected:
	vector<DraggablePoint> points;
	set<unsigned int> selected;
	
	ofVec2f mouseStart, positionStart;
	float pointSize, clickRadiusSquared;
	
public:
	DraggablePoints()
	:clickRadiusSquared(0) {
	}
	unsigned int size() {
		return points.size();
	}
	void add(const ofVec2f& v) {
		points.push_back(DraggablePoint());
		points.back().position = v;
	}
	void setClickRadius(float clickRadius) {
		this->clickRadiusSquared = clickRadius * clickRadius;
	}
	void mousePressed(ofMouseEventArgs& mouse) {
		bool shift = ofGetKeyPressed(OF_KEY_SHIFT);
		bool hitAny = false;
		for(int i = 0; i < size(); i++) {
			bool hit = points[i].isHit(mouse, clickRadiusSquared);
			if(hit && (shift || !hitAny)) {
				points[i].selected = true;
				selected.insert(i);
				hitAny = true;
			} else if(!shift) {
				points[i].selected = false;
				selected.erase(i);
			}
		}
		mouseStart = mouse;
		cachePositions();
	}
	void mouseDragged(ofMouseEventArgs& mouse) {
		ofVec2f offset = mouse - mouseStart;
		for(set<unsigned int>::iterator itr = selected.begin(); itr != selected.end(); itr++) {
			points[*itr].position = points[*itr].positionStart + offset;
		}
	}
	void mouseReleased(ofMouseEventArgs& mouse) {
		cachePositions();
	}
	void keyPressed(ofKeyEventArgs& key) {
		if(isDirectionKey(key.key)) {
			float multiplier = ofGetKeyPressed(OF_KEY_COMMAND) ? .25 : 1;
			ofVec2f offset = multiplier * getDirectionFromKey(key.key);
			for(set<unsigned int>::iterator itr = selected.begin(); itr != selected.end(); itr++) {
				points[*itr].position += offset;
			}
		}
	}
	void cachePositions() {
		for(set<unsigned int>::iterator itr = selected.begin(); itr != selected.end(); itr++) {
			points[*itr].positionStart = points[*itr].position;
		}
	}
	void draw(ofEventArgs& args) {
		for(int i = 0; i < size(); i++) {
			points[i].draw(clickRadiusSquared);
		}
	}
};

class ofApp : public ofBaseApp {
public:
	DraggablePoints points;
	
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