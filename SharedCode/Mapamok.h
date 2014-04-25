#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class Mapamok {
public:
	cv::Mat rvec, tvec;
	ofMatrix4x4 modelMatrix;
	ofxCv::Intrinsics intrinsics;
	bool calibrationReady;
    
    void update(int width, int height, vector<ofVec2f>& imagePoints, vector<ofVec3f>& objectPoints) {
        int n = imagePoints.size();
        const static int minPoints = 6;
        if(n < minPoints) {
            calibrationReady = false;
            return;
        }
        vector<cv::Mat> rvecs, tvecs;
        cv::Mat distCoeffs;
        vector<vector<cv::Point3f> > objectPointsCv(1);
        vector<vector<cv::Point2f> > imagePointsCv(1);
        for(int i = 0; i < n; i++) {
            objectPointsCv[0].push_back(ofxCv::toCv(objectPoints[i]));
            imagePointsCv[0].push_back(ofxCv::toCv(imagePoints[i]));
        }
        float aov = 80; // decent guess
        cv::Size2i imageSize(width, height);
        float f = imageSize.width * ofDegToRad(aov); // this might be wrong, but it's optimized out
        cv::Point2f c = cv::Point2f(imageSize) * (1. / 2);
        cv::Mat1d cameraMatrix = (cv::Mat1d(3, 3) <<
                                  f, 0, c.x,
                                  0, f, c.y,
                                  0, 0, 1);
        int flags =
		CV_CALIB_USE_INTRINSIC_GUESS |
//		CV_CALIB_FIX_PRINCIPAL_POINT |
		CV_CALIB_FIX_ASPECT_RATIO |
		CV_CALIB_FIX_K1 |
		CV_CALIB_FIX_K2 |
		CV_CALIB_FIX_K3;// |
        //		CV_CALIB_ZERO_TANGENT_DIST);
        calibrateCamera(objectPointsCv, imagePointsCv, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, flags);
        rvec = rvecs[0];
        tvec = tvecs[0];
        intrinsics.setup(cameraMatrix, imageSize);
        modelMatrix = ofxCv::makeMatrix(rvec, tvec);
        calibrationReady = true;
    }
    void begin() {
        if(calibrationReady) {
            glPushMatrix();
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glMatrixMode(GL_MODELVIEW);
            intrinsics.loadProjectionMatrix(.1, 10);
            ofxCv::applyMatrix(modelMatrix);
        }
    }
    void end() {
        if(calibrationReady) {
            glPopMatrix();
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
        }
    }
};