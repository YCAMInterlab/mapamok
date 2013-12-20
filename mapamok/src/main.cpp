#include "ofMain.h"

#include "ofxUI.h"
#include "DraggablePoints.h"

class ofApp : public ofBaseApp {
public:
	ofxUICanvas* gui;
	ofxUIRadio* renderMode;
	
	bool editToggle = true;
	bool loadButton = false;
	bool saveButton = false;
	float brightness = 0;
	bool useShader = false;
	
	void setup() {
		setupGui();
	}
	void setupGui() {
		gui = new ofxUICanvas();
		
		ofColor
		cb(64, 192),
		co(192, 192),
		coh(128, 192),
		cf(240, 255),
		cfh(128, 255),
		cp(96, 192),
		cpo(255, 192);
		gui->setUIColors(cb, co, coh, cf, cfh, cp, cpo);
		gui->addLabel("mapamok", OFX_UI_FONT_SMALL);
		
		gui->addSpacer();
		gui->addLabel("Calibration");
		gui->addToggle("Edit", &editToggle);
		gui->addButton("Load", &loadButton);
		gui->addButton("Save", &saveButton);
		
		gui->addSpacer();
		gui->addLabel("Drawing");
		gui->addMinimalSlider("Background", 0, 1, &brightness);
		gui->addToggle("Use shader", &useShader);
		
		gui->addSpacer();
		gui->addLabel("Render");
		vector<string> renderModes;
		renderModes.push_back("Outline");
		renderModes.push_back("Occluded wireframe");
		renderModes.push_back("Full wireframe");
		renderModes.push_back("Faces");
		renderMode = gui->addRadio("Render", renderModes, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
		renderMode->activateToggle(renderModes[0]);
		
		gui->autoSizeToFitWidgets();
	}
	void draw() {
		ofBackground(0);
	}
};

int main() {
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}