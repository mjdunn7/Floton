#include <iostream>
#include <Eigen/Dense>
#include <chrono>
#include "DriverParser.h"
#include "Object3D.h"
#include "CameraSpecifications.h"
#include "CameraPictureTaker.h"

using Eigen::MatrixXd;
int main(int argc, char* argv[]) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 3) {
        std::cout << "invalid command line usage" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    std::string outPutFileName = argv[2];
    DriverParser parser(fileName);
    parser.parse();

    CameraSpecification *specs = parser.getCameraSpecs();

    std::vector<Object3D> models;

    for (int i = 0; i < parser.getTransformations()->size(); ++i) {
        Object3D object3D(parser.getTransformations()->at(i).getFileName());
        object3D.performTransformation(&parser.getTransformations()->at(i));

        models.push_back(object3D);
    }

    CameraPictureTaker imageMaker(specs, &models, parser.getLighting(), parser.getSpheres());
    imageMaker.printImageToFile(outPutFileName);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Execution time: "  << (double) std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0 << 's' << std::endl;
}