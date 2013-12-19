#pragma once

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