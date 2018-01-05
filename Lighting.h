//
// Created by mdunn on 11/1/17.
//

#ifndef CS410_LIGHTING_H
#define CS410_LIGHTING_H


#include <vector>
#include "Light.h"

class Lighting {
private:
    std::vector<Light> lights;
    Eigen::Vector3d ambience;
public:
    Lighting() {

    }

    inline void addLight(double& x, double& y, double& z, double& w, double& r, double& g, double& b){
        Light temp(x, y, z, w, r, g, b);
        lights.push_back(temp);
    }

    inline void setAmbience(double& r, double& g, double& b ){
        ambience << r, g, b;
    }

    inline Eigen::Vector3d& getAmbience(){
        return ambience;
    }

    inline std::vector<Light>& getLights(){
        return lights;
    }
};


#endif //CS410_LIGHTING_H
