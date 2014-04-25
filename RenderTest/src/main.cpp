#include "ofMain.h"
#include "ofAppGLFWWindow.h"

#include "ofxAssimpModelLoader.h"
#include "ofAutoShader.h"
#include "MeshUtils.h"

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
        ofSetVerticalSync(false);
        model.loadModel("model.dae");
        shader.setup("Outline");
        mesh = collapseModel(model);
        mesh = convertIndices(mesh);
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        mesh.clearColors();
        int n = mesh.getNumVertices();
        for(int i = 0; i < n; i++) {
            mesh.addColor(ofColor(255, 0, 0));
            mesh.addColor(ofColor(0, 255, 0));
            mesh.addColor(ofColor(0, 0, 255));
        }
    }
    void update() {
    }
    void draw() {
        ofEnableDepthTest();
        ofBackground(0);
            cam.begin();
            shader.begin();
            mesh.drawFaces();
            shader.end();
            cam.end();
        ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, ofGetHeight() - 40);
    }
};

int main() {
    ofAppGLFWWindow window;
	ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);
    ofRunApp(new ofApp());
}
