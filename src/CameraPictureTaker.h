//
// Created by mdunn on 10/30/17.
//

#ifndef CS410_CAMERPICTURETAKER_H
#define CS410_CAMERPICTURETAKER_H


#include "Object3D.h"
#include "CameraSpecifications.h"
#include "Ray.h"
#include "Sphere.h"
#include "Lighting.h"
#include "RayTraceData.h"

class CameraPictureTaker {
private:
    std::vector<Object3D>* m_models;
    const CameraSpecification* m_specs;

    Eigen::Vector3d m_eyePoint;
    Eigen::Vector3d m_lookVector;
    Eigen::Vector3d m_upVector;
    Eigen::Vector3d m_wVector;
    Eigen::Vector3d m_uVector;
    Eigen::Vector3d m_vVector;

    //Array of pixel distances
    double* m_pixelDistances;

    //Array of strings representing pixels
    std::string* m_pixels;

    int m_height;
    int m_width;

    const std::vector<Sphere>* m_spheres;

    const Lighting* m_lighting;

    //Member functions
    void calculatePixelColors();
    void rayTrace(RayTraceData& data);
    Eigen::Vector3d refractRay(Eigen::Vector3d* rayDirection, Eigen::Vector3d* surfaceNormal, double refractionIndexEnter, double refractionIndexExit);
    bool refract(Eigen::Vector3d* rayStart, Eigen::Vector3d* rayDirection, const Sphere* sphere, double refractionIndexEnter, double refractionIndexExit);
    void processHitSphere(int index, double t, RayTraceData& data);
    void processHitTriangle(Triangle* hitTriangle, double t, RayTraceData& data);
    void convertColorToPixel(int& pixelIndex, Eigen::Vector3d* color);
    Eigen::Matrix<double, 3, 1>* generatePixelPoint(int row, int col, double offset);
    Eigen::Matrix<double, 3, 1>* generatePixelPoint(int row, int col); //overloaded function for when the offset is zero.
    void shootRays(int iStart, int iEnd, int jStart, int jEnd);

    bool rayIntersectsSurface(Eigen::Vector3d *rayStart, Eigen::Vector3d *rayDirection);

public:
    CameraPictureTaker(const CameraSpecification* specs, std::vector<Object3D>* models, const Lighting* lighting, const std::vector<Sphere>* spheres);
    ~CameraPictureTaker();

    void printImageToFile(std::string& fileName);
};


#endif //CS410_CAMERPICTURETAKER_H
