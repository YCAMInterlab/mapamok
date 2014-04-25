#pragma once

#include "EventWatcher.h"
#include "DraggablePoint.h"

class SelectablePoints : public EventWatcher {
protected:
	vector<DraggablePoint> points;
	set<unsigned int> selected;
	
	float clickRadiusSquared;
	
public:
	SelectablePoints()
	:clickRadiusSquared(0) {
	}
	unsigned int size() {
		return points.size();
	}
	void add(const ofVec2f& v) {
		points.push_back(DraggablePoint());
		points.back().position = v;
	}
    DraggablePoint& get(int i) {
        return points[i];
    }
    void clear() {
        points.clear();
        selected.clear();
    }
	void setClickRadius(float clickRadius) {
		this->clickRadiusSquared = clickRadius * clickRadius;
	}
	void mousePressed(ofMouseEventArgs& mouse) {
		bool shift = ofGetKeyPressed(OF_KEY_SHIFT);
		bool hitAny = false;
		for(int i = 0; i < size(); i++) {
			bool hit = points[i].isHit(mouse, clickRadiusSquared);
			if(hit && !hitAny) {
				if(!points[i].selected) {
					points[i].selected = true;
					selected.insert(i);
					hitAny = true;
				}
			} else if(!shift) {
				points[i].selected = false;
				selected.erase(i);
			}
		}
	}
    virtual void keyPressed(ofKeyEventArgs& key) {
        if(key.key == OF_KEY_DEL || key.key == OF_KEY_BACKSPACE) {
            set<unsigned int>::iterator itr;
            for(itr = selected.begin(); itr != selected.end(); itr++) {
                points[*itr].reset();
            }
            selected.clear();
        }
    }
	void draw(ofEventArgs& args) {
        ofPushStyle();
		for(int i = 0; i < size(); i++) {
			points[i].draw(clickRadiusSquared);
		}
        ofPopStyle();
	}
};