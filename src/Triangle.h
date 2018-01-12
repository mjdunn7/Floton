//
// Created by mdunn on 10/29/17.
//

#ifndef CS410_TRIANGLE_H
#define CS410_TRIANGLE_H


#include <Eigen/Dense>
#include <iostream>
#include "Material.h"

//TODO: The triangle class is currently a hot mess. Clean it up.
class Triangle {
private:
    Eigen::Vector3d m_aVector;
    Eigen::Vector3d m_bVector;
    Eigen::Vector3d m_cVector;

    int aVertexNumber;
    int bVertexNumber;
    int cVertexNumber;

private:

    Eigen::Vector3d m_aMinusB;

    bool m_isAmBCalc = false;

    bool m_isNormalVectorCalculated = false;
    Eigen::Vector3d m_normalVector;

    Eigen::Vector3d aMinusLook;
    bool isAMinusLookCalculated = false;

    Material* m_material;

public:
    Triangle(){

    }

    ~Triangle(){

    }

    int getAVertexNumber() const {
        return aVertexNumber;
    }

    void setAVertexNumber(int aVertexNumber) {
        Triangle::aVertexNumber = aVertexNumber;
    }

    int getBVertexNumber() const {
        return bVertexNumber;
    }

    void setBVertexNumber(int bVertexNumber) {
        Triangle::bVertexNumber = bVertexNumber;
    }

    int getCVertexNumber() const {
        return cVertexNumber;
    }

    void setCVertexNumber(int cVertexNumber) {
        Triangle::cVertexNumber = cVertexNumber;
    }

    inline void setA(double x, double y, double z){
        m_aVector << x, y, z;
        m_isAmBCalc = false;
        m_isNormalVectorCalculated = false;
    }

    inline void setB(double x, double y, double z){
        m_bVector << x, y, z;
        m_isAmBCalc = false;
        m_isNormalVectorCalculated = false;
    }

    inline void setC(double x, double y, double z){
        m_cVector << x, y, z;
        m_isNormalVectorCalculated = false;
    }

    inline Eigen::Vector3d* getAVector(){
        return &m_aVector;
    }

    inline Eigen::Vector3d* getBVector(){
        return &m_bVector;
    }

    inline Eigen::Vector3d* getCVector(){
        return &m_cVector;
    }

    inline Eigen::Vector3d* getNormalVector(){
        if(m_isNormalVectorCalculated){
            return &m_normalVector;
        }

        Eigen::Vector3d bMinusA = m_bVector - m_aVector;

        Eigen::Vector3d cMinusA = m_cVector - m_aVector;

        m_normalVector =  bMinusA.cross(cMinusA);
        m_normalVector.normalize();

        m_isNormalVectorCalculated = true;
        return &m_normalVector;
    }

    inline Eigen::Vector3d* getAminusB(){
        if(m_isAmBCalc)
            return &m_aMinusB;

        m_aMinusB = m_aVector - m_bVector;
        m_isAmBCalc = true;
        return &m_aMinusB;
    }

    inline void setMaterial(Material* material){
        m_material = material;
    }

    inline Material* getMaterial(){
        return m_material;
    }
};


#endif //CS410_TRIANGLE_H
