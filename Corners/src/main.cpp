#include "ofMain.h"

#include "ofxAssimpModelLoader.h"
#include "MeshUtils.h"

class ofApp : public ofBaseApp {
public:
	ofMesh mesh, controlPoints;
	ofEasyCam cam;
	vector<unsigned int> rankedCorners;
	
	void setup() {
//		loadModel("mesh.3ds");
	}
	void loadModel(string filename) {
		ofxAssimpModelLoader model;
		model.loadModel(filename);
		mesh = collapseModel(model);
		rankedCorners = getRankedCorners(mesh);
	}
	void update() {
		int n = mesh.getNumVertices();
		int start = ofMap(mouseX, 0, ofGetWidth(), 0, n, true);
		controlPoints.clear();
		controlPoints.setMode(OF_PRIMITIVE_POINTS);
		for(int i = start; i < n; i++) {
			unsigned int index = rankedCorners[i];
			controlPoints.addVertex(mesh.getVertex(index));
		}
	}
	void draw() {
		ofBackground(0);
		cam.begin();
		ofSetLineWidth(2);
		ofSetColor(ofColor::red, 16);
		mesh.drawWireframe();
		glPointSize(4);
		ofSetColor(ofColor::white);
		controlPoints.draw();
		cam.end();
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
