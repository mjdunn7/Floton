//
// Created by mdunn on 10/31/17.
//

#ifndef CS410_LIGHT_H
#define CS410_LIGHT_H

#include <Eigen/Dense>


class Light {
private:
    Eigen::Vector3d position;
    Eigen::Vector3d rgb;
public:
    Light(double& x, double& y, double& z, double& w, double& r, double& g, double& b){
        position << x, y, z;
        rgb << r, g, b;
    }

    inline Eigen::Vector3d getPosition(){
        return position;
    }

    inline Eigen::Vector3d getRGB(){
        return rgb;
    }
};


#endif //CS410_LIGHT_H
