//
// Created by mdunn on 10/16/17.
//

#ifndef CS410_OBJECT3D_H
#define CS410_OBJECT3D_H

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "Transformation.h"
#include "Triangle.h"
#include "Material.h"
#include <vector>
#include <map>


class Object3D {
private:
    Eigen::Matrix<double, 4, Eigen::Dynamic> m_verticesMatrix;
    std::vector<Triangle*> m_triangles;
    std::vector<std::string> m_rawFaces;
    std::string m_fileName;

    bool isTrianglesCalculated = false;

    std::vector<Eigen::Matrix<double, 1, 3>> m_vertices;

    Eigen::Vector3d m_rotationZAxis;
    Eigen::Vector3d m_rotationXAxis;
    Eigen::Vector3d m_rotationYAxis;

    //Member functions
    bool parseFile();
    void tokenize(std::string* line, std::vector<std::string>* words, const char& delim);
    void parseMaterialsFile(std::string& fileName);

    std::map<std::string, Material*> materialMap;


public:
    Object3D(std::string fileName){
        m_fileName = fileName;
        parseFile();
    }

    ~Object3D();

    void printToFile();
    void performTransformation(Transformation* transformation);
    void parseFace(std::string &rawFace, std::string& materialName);

    const std::vector<Triangle*>* getTriangles();


};


#endif //CS410_OBJECT3D_H
