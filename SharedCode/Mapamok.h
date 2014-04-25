/*
 Mapamok class.
 */

#pragma once

#include "ofMain.h"

class Mapamok {
public:
    bool calibrationReady = false;
    
    void update(vector<ofVec2f>& imagePoints, vector<ofVec3f>& objectPoints) {
        int n = imagePoints.size();
//        const static int minPoints = 6;
//        if(n < minPoints) {
//            calibrationReady = false;
//            return;
//        }
//        vector<Mat> rvecs, tvecs;
//        Mat distCoeffs;
//        vector<vector<Point3f> > referenceObjectPoints(1);
//        vector<vector<Point2f> > referenceImagePoints(1);
//        for(int i = 0; i < n; i++) {
//            if(referencePoints[i]) {
//                referenceObjectPoints[0].push_back(objectPoints[i]);
//                referenceImagePoints[0].push_back(imagePoints[i]);
//            }
//        }
//        calibrateCamera(referenceObjectPoints, referenceImagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, flags);
//        rvec = rvecs[0];
//        tvec = tvecs[0];
//        intrinsics.setup(cameraMatrix, imageSize);
//        modelMatrix = makeMatrix(rvec, tvec);
//        calibrationReady = true;
    }
};