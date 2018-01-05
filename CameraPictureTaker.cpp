//
// Created by mdunn on 10/30/17.
//

#include <fstream>
#include <iostream>
#include "CameraPictureTaker.h"
#include "Sphere.h"
#include "Lighting.h"

CameraPictureTaker::CameraPictureTaker(CameraSpecification *specs, std::vector<Object3D>* models, Lighting* lighting, std::vector<Sphere>* spheres) {
    m_specs = specs;
    m_lighting = lighting;

    m_eyePoint << specs->eyeX, specs->eyeY, specs->eyeZ;
    m_lookVector << specs ->lookX, specs->lookY, specs->lookZ;
    m_upVector << specs->upX, specs->upY, specs->upZ;

    m_wVector = -1.0 * (m_eyePoint - m_lookVector);
    m_wVector.normalize();

    m_uVector = m_upVector.cross(m_wVector);
    m_uVector.normalize();

    m_vVector = m_wVector.cross(m_uVector);
    m_vVector.normalize();

    m_height = specs->imageHeight;
    m_width = specs->imageWidth;

    m_pixelDistances = new double[m_width * m_height];

    m_spheres = spheres;

    m_pixels = new std::string[m_width * m_height];

    for(int i = 0; i < m_width *m_height; ++i){
        m_pixels[i] = "0 0 0";
    }

    if(models == nullptr){
        m_models = nullptr;
    }else {
        m_models = models;
    }

    calculateDistances();

}

CameraPictureTaker::~CameraPictureTaker() {
    delete [] m_pixelDistances;
    delete [] m_pixels;
}

Eigen::Matrix<double, 3, 1> *CameraPictureTaker::generatePixelPoint(int row, int col, double offset) {
    double px = ((double)row)/(m_width - 1.0) * (m_specs->boundRight - m_specs->boundLeft) + m_specs->boundLeft;
    double py = ((double)col)/(m_height - 1.0) * (m_specs->boundTop - m_specs->boundBottom) + m_specs->boundBottom;

    Eigen::Matrix<double, 3, 1>* pixel = new Eigen::Matrix<double,3,1>;

    if(offset != 0) {
        double randomXOffset =
                static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (offset * 2)) - offset);
        double randomYOffset =
                static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (offset * 2)) - offset);
        *pixel = m_eyePoint + (m_specs->focalLength * m_wVector) + ((px + randomYOffset) * m_uVector) +
                 ((py + randomXOffset) * m_vVector);
    }else{
        *pixel = m_eyePoint + (m_specs->focalLength * m_wVector) + (px * m_uVector) +
                 (py * m_vVector);
    }
    return pixel;
}

Eigen::Matrix<double, 3, 1> *CameraPictureTaker::generatePixelPoint(int row, int col) {
    double px = ((double)row)/(m_width - 1.0) * (m_specs->boundRight - m_specs->boundLeft) + m_specs->boundLeft;
    double py = ((double)col)/(m_height - 1.0) * (m_specs->boundTop - m_specs->boundBottom) + m_specs->boundBottom;

    Eigen::Matrix<double, 3, 1>* pixel = new Eigen::Matrix<double,3,1>;

    *pixel = m_eyePoint + (m_specs->focalLength * m_wVector) + (px * m_uVector) +
             (py * m_vVector);
    return pixel;
}

void CameraPictureTaker::calculateDistances() {
    double res = m_height * m_width;
    double pixelsCompleted = 0;
    int previousPercentage = 0;
    int percent;
    for (int i = m_height - 1; i > -1; --i) {
        for (int j = m_width - 1; j > -1; --j) {
            percent = (int) pixelsCompleted/res * 100;
            if(percent >= previousPercentage + 10){
                previousPercentage = percent;
                std::cout << percent << "% complete." << std::endl;
            }
            int index = i * m_width + j;

            if(m_specs->focusOn) {
                Eigen::Vector3d grandAccumulator;
                grandAccumulator.setZero();

                Eigen::Matrix<double, 3, 1> *pixelPoint = generatePixelPoint(j, i, 0);
                Eigen::Vector3d directionVector = *pixelPoint - m_eyePoint;
                for (int k = 0; k < m_specs->focusRaysPerPixel; ++k) {
                    Eigen::Matrix<double, 3, 1> *offsetPixelPoint = generatePixelPoint(j, i, m_specs->focusSpread);

                    directionVector.normalize();
                    Eigen::Vector3d pointInFocus = *pixelPoint + (m_specs->focusDistance * directionVector);

                    Ray ray(offsetPixelPoint, pointInFocus - *offsetPixelPoint);

                    Eigen::Vector3d accumulator;
                    accumulator.setZero();
                    Eigen::Vector3d refatt;
                    refatt.setConstant(1);
                    m_attenuation.setConstant(1);
                    m_color.setZero();

                    rayTrace(*ray.getStartPoint(), *ray.getDirectionVector(),
                             refatt, &accumulator, m_specs->recursionLevel);

                    grandAccumulator += accumulator;
                }
                delete pixelPoint;
                ++pixelsCompleted;
                grandAccumulator = grandAccumulator / m_specs->focusRaysPerPixel;
                convertColorToPixel(index, &grandAccumulator);

            }else{
                Eigen::Matrix<double, 3, 1> *pixelPoint = generatePixelPoint(j, i, 0);
                Ray ray(pixelPoint, *pixelPoint - m_eyePoint);

                Eigen::Vector3d accumulator;
                accumulator.setZero();
                Eigen::Vector3d refatt;
                refatt << 1, 1, 1;
                m_attenuation << 1, 1, 1;
                m_color << 0, 0, 0;

                rayTrace(*ray.getStartPoint(), *ray.getDirectionVector(),
                         refatt, &accumulator, m_specs->recursionLevel);
                ++pixelsCompleted;
                convertColorToPixel(index, &accumulator);
            }
        }
    }

    std::cout << "100% complete." << std::endl;
}

void CameraPictureTaker::printImageToFile(std::string &fileName) {
    using namespace std;
    ofstream file;
    file.open(fileName, std::ofstream::out | std::ofstream::ate);
    string header = "P3\n" + to_string(m_width) + " " + to_string(m_height) + "\n255\n";
    file << header;

    for (int i = m_height - 1; i > -1; --i) {
        for (int j = m_width - 1; j > -1; --j) {
            file << m_pixels[i * m_width + j] + " ";
        }
        file << '\n';
    }
}

void CameraPictureTaker::convertColorToPixel(int &pixelIndex, Eigen::Vector3d *color) {
    std::string sr;
    std::string sg;
    std::string sb;
    std::ostringstream strs;

    int red = std::max(0, std::min(255, (int) round(255.0 * (*color)(0))));
    int green = std::max(0, std::min(255, (int) round(255.0 * (*color)(1))));
    int blue = std::max(0, std::min(255, (int) round(255.0 * (*color)(2))));
    strs << red;
    sr = strs.str();
    strs.str("");
    strs.clear();

    strs << green;
    sg = strs.str();
    strs.str("");
    strs.clear();

    strs << blue;
    sb = strs.str();
    strs.str("");
    strs.clear();

    m_pixels[pixelIndex] = sr + " " + sg + " " + sb;
}

void CameraPictureTaker::rayTrace(Eigen::Vector3d rayStart, Eigen::Vector3d rayDirection, Eigen::Vector3d refatt, Eigen::Vector3d* accumulator,  int level) {
    if(refatt.isZero(.0001)) {
        return;
    }

    Eigen::Matrix<double, 3, 3> solvingMatrix;
    Triangle *curTri;

    Eigen::Matrix<double, 3, 3> solve1;
    Eigen::Matrix<double, 3, 3> solve2;
    Eigen::Matrix<double, 3, 3> solve3;
    Eigen::Matrix<double, 3, 1> aMinusStart;
    m_normalVector << 0, 0, 0;
    m_pointToCameraV << 0, 0, 0;
    m_attenuation << 1, 1, 1;
    m_color << 0, 0, 0;
    Sphere* hitSphere;

    Eigen::Vector3d tV;

    double detM, det1, det2, det3, sbeta, sgamma, newt, t;

    bool didHitSphere = false;
    bool didHitTriangle = false;

    int sphereHitIndex;

    t = -1;

    Triangle* hitTriangle;

    //for every sphere
    for (int k = 0; k < m_spheres->size(); ++k) {
        tV = *m_spheres->at(k).getCenterPoint() - rayStart;
        double v = tV.dot(rayDirection);
        double csq = tV.dot(tV);

        double radius = m_spheres->at(k).getRadius();
        double disk = radius * radius - (csq - v * v);

        if (disk <= 0) {
            continue; //Ray does not intersect sphere
        }

        newt = v - sqrt(disk);

        if(newt <= 0.00001)
            continue;
        if (newt < t || t == -1) {
            //this is an intersection point
            didHitTriangle = false;
            didHitSphere = true;
            sphereHitIndex = k;
            hitSphere = &m_spheres->at(k);

            t = newt;
        }

    }

    //for every model
    for (int m = 0; m < m_models->size(); ++m) {
        std::vector<Triangle *> *triangles = m_models->at(m).getTriangles();

        //for every triangle in the model
        for (int k = 0; k < triangles->size(); ++k) {
            curTri = (*triangles)[k];

            solvingMatrix.col(0) = *curTri->getAminusB();

            solvingMatrix.col(1) = *curTri->getAVector() - *curTri->getCVector();

            solvingMatrix.col(2) = rayDirection.col(0);

            aMinusStart = *curTri->getAVector() - rayStart;

            solve1 = solvingMatrix;
            solve1.col(0) = aMinusStart.col(0);

            solve2 = solvingMatrix;
            solve2.col(1) = aMinusStart.col(0);

            solve3 = solvingMatrix;
            solve3.col(2) = aMinusStart.col(0);

            detM = solvingMatrix.determinant();

            det1 = solve1.determinant();
            sbeta = det1 / detM;
            if (sbeta < 0)
                continue;

            det2 = solve2.determinant();
            sgamma = det2 / detM;
            if (sgamma < 0)
                continue;

            if ((sbeta + sgamma) > 1)
                continue;

            det3 = solve3.determinant();
            newt = det3 / detM;

            if (newt <= 0.001)
                continue;

            if (newt < t || t == -1) {
                //we hit an intersection
                t = newt;
                didHitSphere = false;
                didHitTriangle = true;
                hitTriangle = curTri;
            }

        }

    }

    Eigen::Vector3d intersectionPoint;
    if(didHitTriangle){
        processHitTriangle(hitTriangle, t, rayStart, rayDirection, intersectionPoint);
    }
    if(didHitSphere){
        processHitSphere(sphereHitIndex, t, rayStart, rayDirection, intersectionPoint);
    }

    if(t != -1){
        //we hit a point
        if(level > 0){
            Eigen::Vector3d reflectionRay = (2 * m_normalVector.dot(m_pointToCameraV) * m_normalVector) - m_pointToCameraV;
            reflectionRay.normalize();
            if(didHitSphere){
                *accumulator +=  refatt.cwiseProduct(m_color.cwiseProduct(*hitSphere->getOpacity()));

                refatt = m_attenuation.cwiseProduct(refatt);
                Eigen::Vector3d newAccumulator;
                newAccumulator << 0, 0, 0;
                rayTrace(intersectionPoint, reflectionRay, refatt, &newAccumulator, level-1);
                *accumulator += newAccumulator.cwiseProduct(*hitSphere->getOpacity());
            }else {
                *accumulator += refatt.cwiseProduct(m_color);
                refatt = m_attenuation.cwiseProduct(refatt);
                m_color << 0, 0, 0;
                rayTrace(intersectionPoint, reflectionRay, refatt, accumulator,  level-1);
            }

            if(didHitSphere && m_spheres->at(sphereHitIndex).getOpacitySum() < 3.0){
                rayDirection = -rayDirection;
                if(refract(&intersectionPoint, &rayDirection, hitSphere, hitSphere->getRefractionIndex() , 1.0)){
                    Eigen::Vector3d newAccumulator;
                    newAccumulator << 0, 0, 0;
                    rayTrace(intersectionPoint, rayDirection, refatt, &newAccumulator, level-1);
                    Eigen::Vector3d temp;
                    temp << 1, 1, 1;
                    *accumulator += refatt.cwiseProduct(newAccumulator).cwiseProduct(temp - *hitSphere->getOpacity() );
                    refatt = m_attenuation.cwiseProduct(refatt);
                }
            }
        }else{
            *accumulator += refatt.cwiseProduct(m_color);
        }
    }
}

bool CameraPictureTaker::rayIntersectsSurface(Eigen::Vector3d *rayStart, Eigen::Vector3d *rayDirection) {
    Eigen::Matrix<double, 3, 3> solvingMatrix;
    Triangle *curTri;

    Eigen::Matrix<double, 3, 3> solve1;
    Eigen::Matrix<double, 3, 3> solve2;
    Eigen::Matrix<double, 3, 3> solve3;
    Eigen::Matrix<double, 3, 1> aMinusStart;
    Eigen::Vector3d intersectionPoint;
    Eigen::Vector3d normalVector;

    Eigen::Vector3d tV;

    double detM, det1, det2, det3, sbeta, sgamma, newt;

    //for every sphere
    for (int k = 0; k < m_spheres->size(); ++k) {
        tV = *m_spheres->at(k).getCenterPoint() - *rayStart;
        double v = tV.dot(*rayDirection);
        double csq = tV.dot(tV);

        double radius = m_spheres->at(k).getRadius();
        double disk = radius * radius - (csq - v * v);

        if (disk <= 0001) {
            continue; //Ray does not intersect sphere
        }

        newt = v - sqrt(disk);

        if(newt <= 0.001)
            continue;

        return true;
    }

    //for every model
    for (int m = 0; m < m_models->size(); ++m) {
        std::vector<Triangle *> *triangles = m_models->at(m).getTriangles();

        //for every triangle in the model
        for (int k = 0; k < triangles->size(); ++k) {
            curTri = (*triangles)[k];

            solvingMatrix.col(0) = *curTri->getAminusB();

            solvingMatrix.col(1) = *curTri->getAVector() - *curTri->getCVector();

            solvingMatrix.col(2) = rayDirection->col(0);

            aMinusStart = *curTri->getAVector() - *rayStart;

            solve1 = solvingMatrix;
            solve1.col(0) = aMinusStart.col(0);

            solve2 = solvingMatrix;
            solve2.col(1) = aMinusStart.col(0);

            solve3 = solvingMatrix;
            solve3.col(2) = aMinusStart.col(0);

            detM = solvingMatrix.determinant();

            det1 = solve1.determinant();
            sbeta = det1 / detM;
            if (sbeta < 0.001)
                continue;

            det2 = solve2.determinant();
            sgamma = det2 / detM;
            if (sgamma < 0.001)
                continue;

            if ((sbeta + sgamma) > 0)
                continue;

            det3 = solve3.determinant();
            newt = det3 / detM;

            if (newt <= 0.001)
                continue;

            //we hit an intersection
            return true;

        }

    }
    return false;
}

bool CameraPictureTaker::refract(Eigen::Vector3d* rayStart, Eigen::Vector3d* rayDirection, Sphere* sphere, double refractionIndexEnter, double refractionIndexExit) {
    Eigen::Vector3d norm1 = *rayStart - *sphere->getCenterPoint();
    norm1.normalize();
    Eigen::Vector3d T1 = refractRay(rayDirection, &norm1, refractionIndexExit, refractionIndexEnter);
    if(T1.isZero(0.00001)){
        return false;
    }

    *rayStart = *rayStart + 2 * T1.dot(*sphere->getCenterPoint() - *rayStart) * T1;
    Eigen::Vector3d Nin = *sphere->getCenterPoint() - *rayStart;
    Nin.normalize();
    T1 = -T1;
    *rayDirection = refractRay(&T1, &Nin, refractionIndexEnter, refractionIndexExit);
    return true;

}

Eigen::Vector3d CameraPictureTaker::refractRay(Eigen::Vector3d *rayDirection, Eigen::Vector3d* surfaceNormal,
                                    double refractionIndexEnter, double refractionIndexExit) {
    double refractionRatio = refractionIndexEnter / refractionIndexExit;
    double a = -refractionRatio;
    double normalDotLightRay = rayDirection->dot(*surfaceNormal);

    double radsq = pow(refractionRatio, 2) * (pow(normalDotLightRay, 2) -1) +1;
    if(radsq < 0.0){
        Eigen::Vector3d result;
        result << 0, 0, 0;
        return result;
    }else{
        double b = (refractionRatio * normalDotLightRay) - sqrt(radsq);
        Eigen::Vector3d T = a * *rayDirection  + b * *surfaceNormal;
        return T;
    }

}

void CameraPictureTaker::processHitSphere(int index, double t, Eigen::Vector3d &rayStart, Eigen::Vector3d &rayDirection, Eigen::Vector3d& intersectionPoint){
    intersectionPoint = rayStart + (t * rayDirection);

    m_normalVector = intersectionPoint - *m_spheres->at(index).getCenterPoint();
    m_normalVector.normalize();

    m_pointToCameraV = rayStart - intersectionPoint;
    m_pointToCameraV.normalize();

    //Expected result of the following operation is  <amb1*ka1, amb2*ka2, amb3*ka3>
    m_color = m_spheres->at(index).getAmbientCoef()->cwiseProduct(m_lighting->getAmbience());

    m_attenuation = m_spheres->at(index).getAttenuationCoef();

    for (auto &l : m_lighting->getLights()) {
        Eigen::Vector3d lightPoint = l.getPosition();
        Eigen::Vector3d lightRGBEmission = l.getRGB();

        Eigen::Vector3d lightToPointV = lightPoint - intersectionPoint;
        lightToPointV.normalize();

        if (m_normalVector.dot(lightToPointV) > 0.0 && !rayIntersectsSurface(&intersectionPoint, &lightToPointV )) {
            m_color += m_normalVector.dot(lightToPointV) *
                       m_spheres->at(index).getDiffuseCoef()->cwiseProduct(lightRGBEmission);

            Eigen::Vector3d specularReflection =
                    ((2 * m_normalVector.dot(lightToPointV)) * m_normalVector) - lightToPointV;

            m_color += (pow(m_pointToCameraV.dot(specularReflection), 50.0) *
                        m_spheres->at(index).getSpecularCoef()->cwiseProduct(lightRGBEmission));
        }
    }

}

void CameraPictureTaker::processHitTriangle(Triangle* hitTriangle, double t, Eigen::Vector3d &rayStart,
                                            Eigen::Vector3d &rayDirection, Eigen::Vector3d& intersectionPoint) {
    intersectionPoint = rayStart + (t * rayDirection);
    m_normalVector = *hitTriangle->getNormalVector();

    //Expected result of the following operation is  <amb1*ka1, amb2*ka2, amb3*ka3>
    m_color = hitTriangle->getMaterial()->getAmbientCoeff()->cwiseProduct(
            m_lighting->getAmbience());
    m_attenuation = m_specs->getObjAttenuation();

    m_pointToCameraV = rayStart - intersectionPoint;
    m_pointToCameraV.normalize();

    for (int l = 0; l < m_lighting->getLights().size(); ++l) {
        Eigen::Vector3d lightPoint = m_lighting->getLights().at(l).getPosition();
        Eigen::Vector3d lightRGBEmission = m_lighting->getLights().at(l).getRGB();

        Eigen::Vector3d lightToNormalV =
                lightPoint - m_normalVector; //unit vector in direction of lightPoint to normalVector
        lightToNormalV.normalize();

        if (m_normalVector.dot(lightToNormalV) < 0)
            m_normalVector = -(m_normalVector);

        if (m_normalVector.dot(lightToNormalV) >= 0.0 && !rayIntersectsSurface(&intersectionPoint, &lightToNormalV )) {
            if (m_pointToCameraV.dot(m_normalVector) >= 0.0) {
                //add diffuse color
                m_color += m_normalVector.dot(lightToNormalV) *
                           hitTriangle->getMaterial()->getDiffuseCoeff()->cwiseProduct(lightRGBEmission);
            }

            Eigen::Vector3d specularReflection =
                    ((2 * m_normalVector.dot(lightToNormalV)) * m_normalVector) - lightToNormalV;

            if (m_pointToCameraV.dot(specularReflection) < 0) {
                specularReflection << 0.0, 0.0, 0.0;
            }

            //add specular color
            m_color += (pow(m_pointToCameraV.dot(specularReflection), hitTriangle->getMaterial()->getPhong()) *
                        hitTriangle->getMaterial()->getDiffuseCoeff()->cwiseProduct(lightRGBEmission));

        }

    }

}



