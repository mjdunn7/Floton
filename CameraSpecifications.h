//
// Created by mdunn on 10/30/17.
//

#ifndef CS410_CAMERASPECIFICATIONS_H
#define CS410_CAMERASPECIFICATIONS_H

#include <Eigen/Dense>

class CameraSpecification{
public:
    double eyeX, eyeY, eyeZ;
    double lookX, lookY, lookZ;
    double upX, upY, upZ;
    double focalLength;
    int imageHeight, imageWidth;
    double boundLeft, boundBottom, boundRight, boundTop;
    int recursionLevel = 0;
    bool focusOn = false;
    double focusDistance;
    double focusSpread;
    int focusRaysPerPixel;

    Eigen::Vector3d objAttenuation;

    void setObjAttenuation(double r, double g, double b){
        objAttenuation << r, g, b;
    }

    Eigen::Vector3d getObjAttenuation() const {
        return objAttenuation;
    }
};


#endif //CS410_CAMERASPECIFICATIONS_H
