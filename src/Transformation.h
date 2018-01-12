//
// Created by mdunn on 10/17/17.
//

#ifndef CS410_TRANSFORMATION_H
#define CS410_TRANSFORMATION_H

#include <string>

class Transformation {
private:
    double xRotation;
    double yRotation;
    double zRotation;
    double rotationDegree;
    double sizeScaleFactor;
    double xTranslation;
    double yTranslation;
    double zTranslation;
    std::string fileName;

public:
    const std::string &getFileName() const;

    void setFileName(const std::string &fileName);

    double getXRotation() const;

    void setXRotation(double xRotation);

    double getYRotation() const;

    void setYRotation(double yRotation);

    double getZRotation() const;

    void setZRotation(double zRotation);

    double getRotationDegree() const;

    void setRotationDegree(double rotationDegree);

    double getSizeScaleFactor() const;

    void setSizeScaleFactor(double sizeScaleFactor);

    double getXTranslation() const;

    void setXTranslation(double xTranslation);

    double getYTranslation() const;

    void setYTranslation(double yTranslation);

    double getZTranslation() const;

    void setZTranslation(double zTranslation);
};



#endif //CS410_TRANSFORMATION_H
