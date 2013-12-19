#pragma once

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