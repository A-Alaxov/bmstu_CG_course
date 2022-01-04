#include "illuminant.hpp"
#include "config.hpp"

std::vector<std::vector<double>> &Illuminant::getShadowMap() { return shadowMap; }

Illuminant::Illuminant(Eigen::Matrix4f &transMatrix_)
{
    transMatrix = transMatrix_;
    for (size_t i = 0; i < ILLUM_VIS_X; i++)
    { shadowMap.push_back(std::vector<double>(ILLUM_VIS_Y, 0)); }
}

void Illuminant::setShadowMap(std::vector<std::vector<double>> &setShadowMap)
{
    shadowMap = setShadowMap;
}

void Illuminant::clearShadowMap()
{
    for (size_t i = 0; i < shadowMap.size(); i++)
    {
        for (size_t j = 0; j < shadowMap.at(0).size(); j++)
            shadowMap.at(i).at(j) = 0;
    }
}

void Illuminant::setAngles(int xAngle_, int yAngle_)
{
    xAngle = xAngle_;
    yAngle = yAngle_;
}

int Illuminant::getXAngle() { return xAngle; }

int Illuminant::getYAngle() { return yAngle; }

Eigen::Matrix4f &Illuminant::getTransMat() { return transMatrix; }

void Illuminant::setTransMat(Eigen::Matrix4f &mat) { transMatrix = mat; }
