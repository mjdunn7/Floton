//
// Created by mdunn on 10/30/17.
//

#ifndef CS410_RAY_H
#define CS410_RAY_H


#include <Eigen/Dense>

class Ray {
private:
    Eigen::Vector3d* m_startPoint;
    Eigen::Vector3d m_directionVector;
    double t;

public:
    Ray(Eigen::Vector3d* startPoint, Eigen::Vector3d directionVector);
    ~Ray();

    Eigen::Vector3d* getDirectionVector();
    inline Eigen::Vector3d* getStartPoint(){return m_startPoint;}

};


#endif //CS410_RAY_H
