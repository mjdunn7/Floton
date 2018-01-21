//
// Created by mdunn on 10/16/17.
//

#include <fstream>
#include <string>
#include <iostream>
#include "Object3D.h"

std::string Object3D::getPathName(const std::string& fileName ){
    size_t i = fileName.rfind('/', fileName.length());
    if(i != std::string::npos){
        return fileName.substr(0, i);
    }

    return "";
}

bool Object3D::parseFile() {
    using namespace std;
    ifstream file;
    file.open(m_fileName);
    std::string pathName = getPathName(m_fileName);

    if(!file){
        return false;
    }

    string line;
    string currentMtlName;
    try {
        while (getline(file, line)) {
            //ignore comments
            if (line.find('#') != std::string::npos) {
                line = line.substr(0, line.find('#'));
            }

            int index = 0;
            bool isOnlyWhiteSpace = false;
            while (isspace(line[index])) {
                ++index;
                if (index == line.size() - 1) {
                    //line is only whitespace
                    isOnlyWhiteSpace = true;
                    break;
                }
            }

            if (isOnlyWhiteSpace) {
                continue;
            }

            if(line.empty())
                continue;

            vector<string> words;
            tokenize(&line, &words, ' ');

            if (words[0] == "v") {
                //this line is a vertex

                double x = stod(words[1]);
                double y = stod(words[2]);
                double z = stod(words[3]);

                m_verticesMatrix.conservativeResize(m_verticesMatrix.rows(), m_verticesMatrix.cols() + 1);
                m_verticesMatrix.col(m_verticesMatrix.cols() - 1) << x, y, z, 1;

                continue;
            }

            if (words[0] == "f") {
                m_rawFaces.push_back(line);
                parseFace(line, currentMtlName);
                continue;
            }

            if (words[0] == "mtllib") {
                if(pathName != ""){
                    string materialsFileName = pathName + "/" + words[1];
                    parseMaterialsFile(materialsFileName);
                } else{
                    string materialsFileName = words[1];
                    parseMaterialsFile(materialsFileName);
                }

                continue;
            }

            if (words[0] == "usemtl") {
                currentMtlName = words[1];
                continue;
            }
        }
    } catch (std::exception e){
        std::cerr << "Error parsing object file invalid format" << std::endl;
    }
}

void Object3D::performTransformation(Transformation *transformation) {
    m_rotationZAxis(0) = transformation->getXRotation();
    m_rotationZAxis(1) = transformation->getYRotation();
    m_rotationZAxis(2) = transformation->getZRotation();

    m_rotationZAxis.normalize();

    int minRow, minCol;
    m_rotationZAxis.minCoeff(&minRow, &minCol);
    Eigen::Vector3d nonParallelZ = m_rotationZAxis;
    nonParallelZ(minRow) = 1.0;

    m_rotationXAxis = m_rotationZAxis.cross(nonParallelZ);
    m_rotationYAxis = m_rotationZAxis.cross(m_rotationXAxis);

    m_rotationXAxis.normalize();
    m_rotationYAxis.normalize();

    Eigen::Matrix<double, 4, 4> rotationMatrix;
    rotationMatrix.row(0) << m_rotationXAxis(0), m_rotationXAxis(1), m_rotationXAxis(2), 0.0;
    rotationMatrix.row(1) << m_rotationYAxis(0), m_rotationYAxis(1), m_rotationYAxis(2), 0.0;
    rotationMatrix.row(2) << m_rotationZAxis(0), m_rotationZAxis(1), m_rotationZAxis(2), 0.0;
    rotationMatrix.row(3) << 0.0, 0.0, 0.0, 1.0;

    Eigen::Matrix<double, 4, 4> rotationTheta;
    rotationTheta.setIdentity();

    double pi = 3.141592653589793;

    double cosineAngle = cos(transformation->getRotationDegree() * pi / 180.0);
    double sinAngle = sin(transformation->getRotationDegree() * pi / 180.0);
    rotationTheta(0, 0) = cosineAngle;
    rotationTheta(0, 1) = -sinAngle;
    rotationTheta(1, 0) = sinAngle;
    rotationTheta(1, 1) = cosineAngle;

    Eigen::Matrix<double, 4, 4> rotationMatrixTranspose = rotationMatrix.transpose();
    rotationMatrix = rotationMatrixTranspose * rotationTheta * rotationMatrix;

    Eigen::Matrix<double, 4, 4> scaleMatrix;
    scaleMatrix.setIdentity();

    double scale = transformation->getSizeScaleFactor();
    scaleMatrix(0, 0) = scale;
    scaleMatrix(1, 1) = scale;
    scaleMatrix(2, 2) = scale;

    Eigen::Matrix<double, 4, 4> translationMatrix;
    translationMatrix.setIdentity();
    translationMatrix(0, 3) = transformation->getXTranslation();
    translationMatrix(1, 3) = transformation->getYTranslation();
    translationMatrix(2, 3) = transformation->getZTranslation();

    Eigen::Matrix<double, 4, 4> transformationMatrix = translationMatrix * scaleMatrix * rotationMatrix;
    m_verticesMatrix = transformationMatrix * m_verticesMatrix;

    isTrianglesCalculated = false;

}

void Object3D::printToFile() {
    using namespace std;
    string newFileName = m_fileName.substr(0, m_fileName.find("."));

    newFileName += "_transformed.obj";

    ofstream file;
    file.open (newFileName, std::ofstream::out | std::ofstream::ate);

    for(int i = 0; i < m_verticesMatrix.cols(); ++i){
        std::ostringstream strs;
        strs << m_verticesMatrix(0, i);
        std::string v1 = strs.str();
        strs.str("");
        strs.clear();

        strs << m_verticesMatrix(1, i);
        std::string v2 = strs.str();
        strs.str("");
        strs.clear();

        strs << m_verticesMatrix(2, i);
        std::string v3 = strs.str();
        strs.str("");
        strs.clear();

        string line;
        line = "v " + v1 + " " + v2 + " " + v3 + "\n";
        file << line;
    }

    for (const auto &m_rawFace : m_rawFaces) {
        file << m_rawFace << "\n";
    }

    file.close();
}


void Object3D::parseFace(std::string &rawFace, std::string& materialName) {
    std::vector<std::string> words;

    tokenize(&rawFace, &words, ' ');
    Triangle * tri = new Triangle();

    std::size_t found = rawFace.find("/");
    if (found==std::string::npos){
        std::string sa = words[1];
        std::string ba = words[2];
        std::string ca = words[3];
        int a = stoi(sa);
        int b = stoi(ba);
        int c = stoi(ca);

        tri->setAVertexNumber(a-1);
        tri->setBVertexNumber(b-1);
        tri->setCVertexNumber(c-1);

        //TODO: set a default material name if the material name is blank or does not exist
        if(materialName.empty()){
            tri->setMaterial(materialMap.begin()->second);
        }else {
            tri->setMaterial(materialMap[materialName]);
        }

        m_triangles.push_back(tri);
        return;
    }


    int index = 0;
    std::string sa;
    while(words[1][index] != '/'){
        sa += words[1][index];
        ++index;
    }

    std::string ba;
    index = 0;
    while(words[2][index] != '/'){
        ba += words[2][index];
        ++index;
    }

    std::string ca;
    index = 0;
    while(words[3][index] != '/'){
        ca += words[3][index];
        ++index;
    }

    int a = stoi(sa);
    int b = stoi(ba);
    int c = stoi(ca);

    tri->setAVertexNumber(a-1);
    tri->setBVertexNumber(b-1);
    tri->setCVertexNumber(c-1);

    //TODO: set a default material name if the material name is blank or does not exist
    if(materialName.empty()){
        tri->setMaterial(materialMap.begin()->second);
    }else {
        tri->setMaterial(materialMap[materialName]);
    }

    m_triangles.push_back(tri);
}

void Object3D::tokenize(std::string* line, std::vector<std::string>* words, const char& delim) {
    using namespace std;
    stringstream ss(*line);
    string item;

    words->clear();
    while (getline(ss, item, delim)) {
        words->push_back(item);
    }
}

Object3D::~Object3D() {
    for (int i = 0; i < m_triangles.size(); ++i) {
        //TODO: Figure out why this crashes when not commented out
        //delete(m_triangles[i]);
    }
}

void Object3D::parseMaterialsFile(std::string& fileName) {
    std::ifstream file(fileName);

    std::string line;
    std::string currentMaterialName;
    while(getline(file, line)) {
        //ignore comments
        if (line.find('#') != std::string::npos) {
            line = line.substr(0, line.find('#'));
        }

        if(line.empty()){
            continue;
        }

        int index = 0;
        bool isOnlyWhiteSpace = false;
        while (isspace(line[index])) {
            ++index;
            if (index == line.size() - 1) {
                //line is only whitespace
                isOnlyWhiteSpace = true;
                break;
            }
        }

        if(isOnlyWhiteSpace){
            continue;
        }

        std::vector<std::string> tokens;
        tokenize(&line, &tokens, ' ');
        std::string startWord = tokens[0];
        std::transform(startWord.begin(), startWord.end(), startWord.begin(), ::tolower);

        if(startWord == "newmtl"){
            currentMaterialName = tokens[1];
            if(materialMap.count(currentMaterialName) > 0){
                delete(materialMap[currentMaterialName]);
            }
            materialMap[currentMaterialName] = new Material(currentMaterialName);
            continue;
        }

        if(startWord == "ka") {
            double r = stod(tokens[1]);
            double g = stod(tokens[2]);
            double b = stod(tokens[3]);

            materialMap[currentMaterialName]->setAmbientCoeff(r,g,b);
            continue;
        }

        if(startWord == "kd"){
            double r = stod(tokens[1]);
            double g = stod(tokens[2]);
            double b = stod(tokens[3]);

            materialMap[currentMaterialName]->setDiffuseCoeff(r,g,b);
            continue;
        }

        if(startWord == "ks"){
            double r = stod(tokens[1]);
            double g = stod(tokens[2]);
            double b = stod(tokens[3]);

            materialMap[currentMaterialName]->setSpecularCoeff(r,g,b);
            continue;
        }

        if(startWord == "ns"){
            double phong = stod(tokens[1]);
            materialMap[currentMaterialName]->setPhong(phong);
            continue;
        }
    }
}

const std::vector<Triangle*>* Object3D::getTriangles() {
    if(isTrianglesCalculated)
        return &m_triangles;

    for(int i = 0; i < m_triangles.size(); ++i){
        int a = m_triangles[i]->getAVertexNumber();
        int b = m_triangles[i]->getBVertexNumber();
        int c = m_triangles[i]->getCVertexNumber();

        m_triangles[i]->setA(m_verticesMatrix(0, a), m_verticesMatrix(1, a), m_verticesMatrix(2, a));
        m_triangles[i]->setB(m_verticesMatrix(0, b), m_verticesMatrix(1, b), m_verticesMatrix(2, b));
        m_triangles[i]->setC(m_verticesMatrix(0, c), m_verticesMatrix(1, c), m_verticesMatrix(2, c));
    }

    isTrianglesCalculated = true;

    return &m_triangles;
}

