#ifndef ILLUMINANT_HPP
#define ILLUMINANT_HPP

#include <vector>
#include <eigen3/Eigen/Dense>

class Illuminant
{
public:
    Illuminant(Eigen::Matrix4f &transMat);
    Illuminant(){};

    void setShadowMap(std::vector<std::vector<double>> &setShadowMap);
    std::vector<std::vector<double>> &getShadowMap();
    void clearShadowMap();

    void setAngles(int xAngle_, int yAngle_);
    int getXAngle();
    int getYAngle();

    void setTransMat(Eigen::Matrix4f &mat);
    Eigen::Matrix4f &getTransMat();

private:
    std::vector<std::vector<double>> shadowMap;
    Eigen::Matrix4f transMatrix;
    int xAngle, yAngle;
};

#endif // ILLUMINANT_HPP
