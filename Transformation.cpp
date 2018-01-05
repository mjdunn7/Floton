//
// Created by mdunn on 10/17/17.
//

#include "Transformation.h"

double Transformation::getXRotation() const {
    return xRotation;
}

void Transformation::setXRotation(double xRotation) {
    Transformation::xRotation = xRotation;
}

double Transformation::getYRotation() const {
    return yRotation;
}

void Transformation::setYRotation(double yRotation) {
    Transformation::yRotation = yRotation;
}

double Transformation::getZRotation() const {
    return zRotation;
}

void Transformation::setZRotation(double zRotation) {
    Transformation::zRotation = zRotation;
}

double Transformation::getRotationDegree() const {
    return rotationDegree;
}

void Transformation::setRotationDegree(double rotationDegree) {
    Transformation::rotationDegree = rotationDegree;
}

double Transformation::getSizeScaleFactor() const {
    return sizeScaleFactor;
}

void Transformation::setSizeScaleFactor(double sizeScaleFactor) {
    Transformation::sizeScaleFactor = sizeScaleFactor;
}

double Transformation::getXTranslation() const {
    return xTranslation;
}

void Transformation::setXTranslation(double xTranslation) {
    Transformation::xTranslation = xTranslation;
}

double Transformation::getYTranslation() const {
    return yTranslation;
}

void Transformation::setYTranslation(double yTranslation) {
    Transformation::yTranslation = yTranslation;
}

double Transformation::getZTranslation() const {
    return zTranslation;
}

void Transformation::setZTranslation(double zTranslation) {
    Transformation::zTranslation = zTranslation;
}

const std::string &Transformation::getFileName() const {
    return fileName;
}

void Transformation::setFileName(const std::string &fileName) {
    Transformation::fileName = fileName;
}
