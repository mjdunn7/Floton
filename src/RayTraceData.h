//
// Created by mdunn on 1/13/18.
//

#ifndef FLOTON_RAYTRACEDATA_H
#define FLOTON_RAYTRACEDATA_H


#include <Eigen/Dense>

//This class exists to encapsulate data that is used during the ray tracing process to make function parameters shorter
//and less confusing.
class RayTraceData {
private:
    Eigen::Vector3d m_normalVector;
    Eigen::Vector3d m_pointToCameraV;
    Eigen::Vector3d m_attenuation;
    Eigen::Vector3d m_color;
    Eigen::Vector3d m_rayStart;
    Eigen::Vector3d m_rayDirection;
    Eigen::Vector3d m_reflectAccumulator;
    Eigen::Vector3d m_colorAccumulator;
    Eigen::Vector3d m_intersectionPoint;

public:
    RayTraceData(){
        m_normalVector.setZero();
        m_pointToCameraV.setZero();
        m_colorAccumulator.setZero();
        m_attenuation.setConstant(1);
        m_color.setZero();
        m_reflectAccumulator.setConstant(1);
        level = 0;
    }

    inline Eigen::Vector3d* getNormalVector(){return &m_normalVector;}
    inline Eigen::Vector3d* getPointToCameraV(){return &m_pointToCameraV;}
    inline Eigen::Vector3d* getAttenuation(){return &m_attenuation;}
    inline Eigen::Vector3d* getColor(){return &m_color;}
    inline Eigen::Vector3d* getRayStart(){return &m_rayStart;}
    inline Eigen::Vector3d* getRayDirection(){return &m_rayDirection;}
    inline Eigen::Vector3d* getReflectAccum(){return &m_reflectAccumulator;}
    inline Eigen::Vector3d* getColorAccum(){return &m_colorAccumulator;}
    inline Eigen::Vector3d* getIntersectionPoint(){return &m_intersectionPoint;}

    int level;
};


#endif //FLOTON_RAYTRACEDATA_H
