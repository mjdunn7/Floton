//
// Created by mdunn on 10/29/17.
//

#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "DriverParser.h"
#include "CameraSpecifications.h"
#include "Sphere.h"

bool DriverParser::parse() {
    std::ifstream file(m_fileName);

    if(!file.is_open()){
        std::cerr << "Invalid driver file specified." << std::endl;
        exit(1);
    }
    std::string line;
    while(getline(file, line)) {
        try {
            //ignore comments
            if (line.find('#') != std::string::npos) {
                line = line.substr(0, line.find('#'));
            }

            if (line.empty()) {
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

            if (isOnlyWhiteSpace) {
                continue;
            }


            tokenize(&line);
            std::string startWord = m_tokens[0];
            std::transform(startWord.begin(), startWord.end(), startWord.begin(), ::tolower);

            if (startWord == "model") {
                Transformation t;
                t.setXRotation(stod(m_tokens[1]));
                t.setYRotation(stod(m_tokens[2]));
                t.setZRotation(stod(m_tokens[3]));
                t.setRotationDegree(stod(m_tokens[4]));
                t.setSizeScaleFactor(stod(m_tokens[5]));
                t.setXTranslation(stod(m_tokens[6]));
                t.setYTranslation(stod(m_tokens[7]));
                t.setZTranslation(stod(m_tokens[8]));
                t.setFileName(m_tokens[9]);
                m_transformations.push_back(t);
                continue;
            }

            if (startWord == "eye") {
                m_cSpecs.eyeX = stod(m_tokens[1]);
                m_cSpecs.eyeY = stod(m_tokens[2]);
                m_cSpecs.eyeZ = stod(m_tokens[3]);
                continue;
            }

            if (startWord == "look") {
                m_cSpecs.lookX = stod(m_tokens[1]);
                m_cSpecs.lookY = stod(m_tokens[2]);
                m_cSpecs.lookZ = stod(m_tokens[3]);
                continue;
            }

            if (startWord == "up") {
                m_cSpecs.upX = stod(m_tokens[1]);
                m_cSpecs.upY = stod(m_tokens[2]);
                m_cSpecs.upZ = stod(m_tokens[3]);
                continue;
            }

            if (startWord == "d") {
                m_cSpecs.focalLength = stod(m_tokens[1]);
                continue;
            }

            if (startWord == "bounds") {
                m_cSpecs.boundLeft = stod(m_tokens[1]);
                m_cSpecs.boundBottom = stod(m_tokens[2]);
                m_cSpecs.boundRight = stod(m_tokens[3]);
                m_cSpecs.boundTop = stod(m_tokens[4]);
                continue;
            }

            if (startWord == "res") {
                m_cSpecs.imageWidth = stoi(m_tokens[1]);
                m_cSpecs.imageHeight = stoi(m_tokens[2]);
                continue;
            }

            if (startWord == "objattn"){
                double r = stod(m_tokens[1]);
                double g = stod(m_tokens[2]);
                double b = stod(m_tokens[3]);

                m_cSpecs.setObjAttenuation(r, g, b);
            }

            if(startWord == "focus"){
                m_cSpecs.focusOn = true;
                m_cSpecs.focusDistance = stod(m_tokens[1]);
                m_cSpecs.focusSpread = stod(m_tokens[2]);
                m_cSpecs.focusRaysPerPixel = stoi(m_tokens[3]);
            }

            if (startWord == "sphere") {
                double cx = stod(m_tokens[1]);
                double cy = stod(m_tokens[2]);
                double cz = stod(m_tokens[3]);
                double radius = stod(m_tokens[4]);
                Sphere sphere(cx, cy, cz, radius);

                double ambientR = stod(m_tokens[5]);
                double ambientG = stod(m_tokens[6]);
                double ambientB = stod(m_tokens[7]);
                sphere.setAmbientCoefficients(ambientR, ambientG, ambientB);

                double diffuseR = stod(m_tokens[8]);
                double diffuseG = stod(m_tokens[9]);
                double diffuseB = stod(m_tokens[10]);
                sphere.setDiffuseCoefficients(diffuseR, diffuseG, diffuseB);

                double specR = stod(m_tokens[11]);
                double specG = stod(m_tokens[12]);
                double specB = stod(m_tokens[13]);
                sphere.setSpecularCoefficients(specR, specG, specB);

                double attenuationR = stod(m_tokens[14]);
                double attenuationG = stod(m_tokens[15]);
                double attenuationB = stod(m_tokens[16]);
                sphere.setAttenuationCoefficients(attenuationR, attenuationG, attenuationB);

                double opacityR = stod(m_tokens[17]);
                double opacityG = stod(m_tokens[18]);
                double opacityB = stod(m_tokens[19]);
                sphere.setOpacity(opacityR, opacityG, opacityB);

                double refractionIndex = stod(m_tokens[20]);
                sphere.setRefractionIndex(refractionIndex);

                m_spheres.push_back(sphere);
                continue;
            }

            if (startWord == "light") {
                double x = stod(m_tokens[1]);
                double y = stod(m_tokens[2]);
                double z = stod(m_tokens[3]);
                double w = stod(m_tokens[4]);
                double r = stod(m_tokens[5]);
                double g = stod(m_tokens[6]);
                double b = stod(m_tokens[7]);

                lighting.addLight(x, y, z, w, r, g, b);
                continue;
            }

            if (startWord == "ambient") {
                double r = stod(m_tokens[1]);
                double g = stod(m_tokens[2]);
                double b = stod(m_tokens[3]);

                lighting.setAmbience(r, g, b);
                continue;
            }

            if(startWord == "recursionlevel"){
                m_cSpecs.recursionLevel = stoi(m_tokens[1]);
                continue;
            }

        } catch (std::exception e){
            std::cerr << "Error parsing driver file. Invalid file format" << std::endl;
            exit(1);

        }
    }
}

void DriverParser::tokenize(std::string* line) {
    using namespace std;
    stringstream ss(*line);
    string item;

    m_tokens.clear();
    while (getline(ss, item, ' ')) {
        m_tokens.push_back(item);
    }
}
