#include "ofMain.h"
#include "ofAppGLFWWindow.h"

#include "ofxAssimpModelLoader.h"
#include "ofAutoShader.h"

ofMesh collapseModel(ofxAssimpModelLoader model) {
    ofMesh mesh;
    for(int i = 0; i < model.getNumMeshes(); i++) {
        ofMesh curMesh = model.getMesh(i);
        mesh.append(curMesh);
    }
    return mesh;
}

class ofApp : public ofBaseApp {
public:
    ofxAssimpModelLoader model;
    ofEasyCam cam;
    ofVboMesh mesh;
    ofAutoShader shader;
    
    void setup() {
//        ofSetVerticalSync(false);
        model.loadModel("model.dae");
        shader.setup("Outline");
        mesh = collapseModel(model);
        ofLog() << mesh.getNumVertices() << " " << mesh.getNumIndices();
    }
    void update() {
    }
    void draw() {
        ofBackground(0);
        if(true) {
            cam.begin();
            shader.begin();
            model.drawFaces();
            shader.end();
            cam.end();
        } else {
            ofPushStyle();
            ofSetLineWidth(4);
            ofLine(0, 0, ofGetWidth(), ofGetHeight());
            ofLine(ofGetWidth(), 0, 0, ofGetHeight());
            ofPopStyle();
        }
        ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, ofGetHeight() - 40);
    }
};

int main() {
    ofAppGLFWWindow window;
	ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);
    ofRunApp(new ofApp());
}
