//
// Created by mdunn on 10/29/17.
//

#ifndef CS410_DRIVERPARSER_H
#define CS410_DRIVERPARSER_H

#include <string>
#include <vector>
#include "Transformation.h"
#include "CameraSpecifications.h"
#include "Sphere.h"
#include "Lighting.h"


class DriverParser {
private:
    std::string m_fileName;
    std::vector<Transformation> m_transformations;
    std::vector<Sphere> m_spheres;
    std::vector<std::string> m_tokens;
    CameraSpecification m_cSpecs;
    Lighting lighting;
    std::string m_mtlFileName;

    //Member functions
    void tokenize(std::string* line);

public:
    DriverParser(std::string& fileName){
        m_fileName = fileName;
    }

    bool parse();

    inline std::vector<Transformation>* getTransformations(){
        return &m_transformations;
    }

    inline CameraSpecification* getCameraSpecs(){
        return &m_cSpecs;
    }

    inline std::vector<Sphere>* getSpheres(){
        return &m_spheres;
    }

    inline Lighting* getLighting(){
        return &lighting;
    }
};


#endif //CS410_DRIVERPARSER_H
