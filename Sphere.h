//
// Created by mdunn on 10/31/17.
//

#ifndef CS410_SPHERE_H
#define CS410_SPHERE_H


#include <Eigen/Dense>

class Sphere {
private:
    double m_radius;
    double m_refractionIndex;

private:
    Eigen::Vector3d m_centerPoint;
    Eigen::Vector3d m_ambientCoefficients;
    Eigen::Vector3d m_diffuseCoefficients;
    Eigen::Vector3d m_specularCoefficients;
    Eigen::Vector3d m_attenuationCoefficients;
    Eigen::Vector3d m_opacityCoefficients;

public:
    Sphere(double cx, double cy, double cz, double radius){
        m_radius = radius;
        m_centerPoint << cx, cy, cz;
        m_opacityCoefficients << 1, 1, 1;
    }

    inline double getRefractionIndex() const {
        return m_refractionIndex;
    }

    inline void setOpacity(double r, double g, double b){
        m_opacityCoefficients << r, g, b;
    }

    inline void setRefractionIndex(double m_refractionIndex) {
        Sphere::m_refractionIndex = m_refractionIndex;
    }

    inline void setAmbientCoefficients(double& a, double& b, double& c){
        m_ambientCoefficients << a, b, c;
    }

    inline void setDiffuseCoefficients(double& a, double& b, double& c){
        m_diffuseCoefficients << a, b, c;
    }

    inline void setSpecularCoefficients(double& a, double& b, double& c){
        m_specularCoefficients << a, b, c;
    }

    inline void setAttenuationCoefficients(double& a, double& b, double& c){
        m_attenuationCoefficients << a, b, c;
    }

    inline Eigen::Vector3d* getCenterPoint(){
        return &m_centerPoint;
    }

    inline Eigen::Vector3d* getOpacity(){
        return &m_opacityCoefficients;
    }

    inline double getOpacitySum(){
        return m_opacityCoefficients(0) + m_opacityCoefficients(1) + m_opacityCoefficients(2);
    }

    inline Eigen::Vector3d* getDiffuseCoef(){
        return &m_diffuseCoefficients;
    }

    inline Eigen::Vector3d* getSpecularCoef(){
        return &m_specularCoefficients;
    }

    inline Eigen::Vector3d getAttenuationCoef(){
        return m_attenuationCoefficients;
    }

    inline Eigen::Vector3d* getAmbientCoef(){
        return &m_ambientCoefficients;
    }

    inline double getRadius(){
        return m_radius;
    }



};


#endif //CS410_SPHERE_H
