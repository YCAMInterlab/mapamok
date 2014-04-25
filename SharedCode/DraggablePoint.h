#pragma once

class DraggablePoint {
public:
	ofVec2f position, positionStart;
	bool selected, dragging, hit;
	
	DraggablePoint()
	:selected(false)
	,dragging(false)
    ,hit(false) {
	}
    void reset() {
        selected = false;
        dragging = false;
        hit = false;
    }
	bool isHit(ofVec2f v, float clickRadiusSquared) {
		bool curHit = position.distanceSquared(v) < clickRadiusSquared;
        if(curHit) {
            hit = true;
        }
        return curHit;
	}
	void draw(float clickRadiusSquared) {
		float r = 2;//sqrt(clickRadiusSquared);
		ofPushStyle();
		ofNoFill();
		ofSetLineWidth(2);
		if(selected) {
			ofSetColor(ofColor::yellow);
			ofCircle(position, r + 4);
            ofSetLineWidth(1);
            ofSetColor(255);
            ofLine(position.x, 0, position.x, ofGetHeight());
            ofLine(0, position.y, ofGetWidth(), position.y);
		}
		ofPopStyle();
		ofPushStyle();
		ofFill();
        if(hit) {
            ofSetColor(ofColor::green);
        }
		ofCircle(position, r);
		ofPopStyle();
	}
};