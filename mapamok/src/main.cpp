#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxUI.h"
#include "DraggablePoints.h"
#include "MeshUtils.h"

class ofApp : public ofBaseApp {
public:
	ofxUICanvas* gui;
	ofxUIRadio* renderMode;
	
	bool editToggle = true;
	bool loadButton = false;
	bool saveButton = false;
	float backgroundBrightness = 0;
	bool useShader = false;
	
	ofxAssimpModelLoader model;
	ofMesh mesh, imageMesh;
	ofEasyCam cam;
	SelectablePoints objectPoints;
		
	void setup() {
		ofSetWindowTitle("mapamok");
		setupGui();
		if(ofFile::doesFileExist("model.dae")) {
			loadModel("model.dae");
		}
		cam.setDistance(1);
		cam.setNearClip(.001);
		cam.setFarClip(100);
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
				
		gui->addSpacer();
		gui->addLabel("Calibration");
		gui->addToggle("Edit", &editToggle);
		gui->addButton("Load", &loadButton);
		gui->addButton("Save", &saveButton);
		
		gui->addSpacer();
		gui->addLabel("Render");
		vector<string> renderModes;
		renderModes.push_back("Outline");
		renderModes.push_back("Occluded wireframe");
		renderModes.push_back("Full wireframe");
		renderModes.push_back("Faces");
		renderMode = gui->addRadio("Render", renderModes, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
		renderMode->activateToggle(renderModes[0]);
		
		gui->addSpacer();
		gui->addMinimalSlider("Background", 0, 255, &backgroundBrightness);
		gui->addToggle("Use shader", &useShader);
		
		gui->autoSizeToFitWidgets();
	}
	void draw() {
		ofBackground(backgroundBrightness);
		cam.begin();
		ofSetColor(128);
		mesh.drawWireframe();
		cam.end();
		
		imageMesh = mesh;
		project(imageMesh, cam, ofGetWindowRect());
		glPointSize(4);
		imageMesh.setMode(OF_PRIMITIVE_POINTS);
		ofSetColor(255, 0, 0);
		imageMesh.draw();
	}
	void loadModel(string filename) {
		model.loadModel(filename);
		mesh = model.getMesh(0);
		centerAndNormalize(mesh);
	}
	void dragEvent(ofDragInfo dragInfo) {
		if(dragInfo.files.size() == 1) {
			string filename = dragInfo.files[0];
			loadModel(filename);
		}
	}
};

int main() {
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}