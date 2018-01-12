//
// Created by mdunn on 11/26/17.
//

#ifndef CS410_MATERIAL_H
#define CS410_MATERIAL_H

#include <Eigen/Dense>
#include <Eigen/Geometry>

class Material {
private:
    Eigen::Vector3d m_ambientCoefficients;
    Eigen::Vector3d m_diffuseCoefficients;
    Eigen::Vector3d m_specularCoefficients;

    std::string m_name;
    double m_phong = 16;


public:
    double getPhong() const {
        return m_phong;
    }

    void setPhong(double m_phong) {
        Material::m_phong = m_phong;
    }

    Material(std::string name){
        m_name = name;
    }

    inline void setAmbientCoeff(double red, double green, double blue){
        m_ambientCoefficients << red, green, blue;
    }

    inline void setDiffuseCoeff(double red, double green, double blue){
        m_diffuseCoefficients << red, green, blue;
    }

    inline void setSpecularCoeff(double red, double green, double blue){
        m_specularCoefficients << red, green, blue;
    }

    inline Eigen::Vector3d* getAmbientCoeff(){
        return &m_ambientCoefficients;
    }

    inline Eigen::Vector3d* getDiffuseCoeff(){
        return &m_diffuseCoefficients;
    }

    inline Eigen::Vector3d* getSpecularCoeff(){
        return &m_specularCoefficients;
    }

};


#endif //CS410_MATERIAL_H
