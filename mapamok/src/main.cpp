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
	ofMesh mesh;
	ofMesh cornerMesh, imageMesh;
	ofEasyCam cam;
	SelectablePoints objectPoints;
		
	void setup() {
		ofSetWindowTitle("mapamok");
		setupGui();
		if(ofFile::doesFileExist("model.dae")) {
			loadModel("model.dae");
		}
		cam.setDistance(1);
		cam.setNearClip(.1);
		cam.setFarClip(10);
	}
	enum {
		RENDER_MODE_WIREFRAME_OCCLUDED = 0,
		RENDER_MODE_WIREFRAME_FULL,
		RENDER_MODE_OUTLINE,
		RENDER_MODE_FACES,
	};
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
		renderModes.push_back("Occluded wireframe");
		renderModes.push_back("Full wireframe");
		renderModes.push_back("Depth Edges");
		renderModes.push_back("Faces");
		renderMode = gui->addRadio("Render", renderModes, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
		renderMode->activateToggle(renderModes[0]);
		
		gui->addSpacer();
		gui->addMinimalSlider("Background", 0, 255, &backgroundBrightness);
		gui->addToggle("Use shader", &useShader);
		
		gui->autoSizeToFitWidgets();
	}
	int getSelection(ofxUIRadio* radio) {
		vector<ofxUIToggle*> toggles = radio->getToggles();
		for(int i = 0; i < toggles.size(); i++) {
			if(toggles[i]->getValue()) {
				return i;
			}
		}
		return -1;
	}
	void draw() {
		ofBackground(backgroundBrightness);
		ofSetColor(255);
		
		cam.begin();
		ofSetLineWidth(2);
		int renderModeSelection = getSelection(renderMode);
		if(renderModeSelection == RENDER_MODE_FACES) {
			ofEnableDepthTest();
			mesh.drawFaces();
			ofDisableDepthTest();
		} else if(renderModeSelection == RENDER_MODE_WIREFRAME_FULL) {
			mesh.drawWireframe();
		} else if(renderModeSelection == RENDER_MODE_OUTLINE || renderModeSelection == RENDER_MODE_WIREFRAME_OCCLUDED) {
			prepareRender(true, true, false);
			glEnable(GL_POLYGON_OFFSET_FILL);
			float lineWidth = ofGetStyle().lineWidth;
			if(renderModeSelection == RENDER_MODE_OUTLINE) {
				glPolygonOffset(-lineWidth, -lineWidth);
			} else if(renderModeSelection == RENDER_MODE_WIREFRAME_OCCLUDED) {
				glPolygonOffset(+lineWidth, +lineWidth);
			}
			glColorMask(false, false, false, false);
			mesh.drawFaces();
			glColorMask(true, true, true, true);
			glDisable(GL_POLYGON_OFFSET_FILL);
			mesh.drawWireframe();
			prepareRender(false, false, false);
		}
		
		ofEnableDepthTest();
		float pointSize = 4;
		glPointSize(pointSize);
		ofSetColor(ofColor::red);
		glEnable(GL_POLYGON_OFFSET_POINT);
		glPolygonOffset(-pointSize, -pointSize);
		cornerMesh.drawVertices();
		glDisable(GL_POLYGON_OFFSET_POINT);
		ofDisableDepthTest();
		
		cam.end();
		
		imageMesh = mesh;
		project(imageMesh, cam, ofGetWindowRect());
		imageMesh.setMode(OF_PRIMITIVE_POINTS);
		ofEnableDepthTest();
//		imageMesh.draw();
		ofDisableDepthTest();
	}
	void loadModel(string filename) {
		model.loadModel(filename);
		mesh = collapseModel(model);
		centerAndNormalize(mesh);
		
		cornerMesh = mesh;
		cornerMesh.clearIndices();
		cornerMesh.setMode(OF_PRIMITIVE_POINTS);
		cornerMesh.addIndices(getCornerVertices(mesh));
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