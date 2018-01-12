//
// Created by mdunn on 10/30/17.
//

#include "Ray.h"

Ray::Ray(Eigen::Matrix<double, 3, 1>*startPoint, Eigen::Matrix<double, 3, 1> directionVector) {
    m_startPoint = startPoint;

    m_directionVector = directionVector;

    m_directionVector.normalize();
}

Ray::~Ray() {
    delete m_startPoint;
}

Eigen::Vector3d* Ray::getDirectionVector() {
    return &m_directionVector;
}
