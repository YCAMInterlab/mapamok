#pragma once

#include "ofMain.h"
#include "ofxCv.h"

ofVec3f ofWorldToScreen(ofVec3f world);
ofVec3f ofScreenToWorld(ofVec3f screen);
ofMesh getProjectedMesh(const ofMesh& mesh);
cv::Point2f getClosestPoint(const vector<cv::Point2f>& vertices, float x, float y, int* choice = NULL, float* distance = NULL);
ofVec3f getClosestPointOnMesh(const ofMesh& mesh, float x, float y, int* choice = NULL, float* distance = NULL);
