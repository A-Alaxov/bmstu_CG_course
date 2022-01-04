#ifndef DOT_H
#define DOT_H

#include <iostream>
#include <vector>

#include "QDebug"
#include <eigen3/Eigen/Dense>
#include "math.h"
#include <set>

class Dot
{
private:
    double x, y, z;
    std::set<size_t> usedFacets;

public:
    Dot(){}
    Dot(double xCoord_, double yCoord_, double zCoord_)
    : x(xCoord_), y(yCoord_), z(zCoord_)
    {
    }
    Dot(const Dot &pos) : x(pos.getX()), y(pos.getY()), z(pos.getZ()), usedFacets(pos.usedFacets) {}

    void setPos(Dot &position_);

    double getX() const;
    void setX(double value);

    double getY() const;
    void setY(double value);

    double getZ() const;
    void setZ(double value);

    void addFacet(size_t fac) { usedFacets.insert(fac); }
    std::set<size_t> getFacets() { return usedFacets; }

    bool operator==(const Dot &p) const;
    Dot operator^(const Dot &p) const;
    Dot operator-(const Dot &p) const;
    Dot operator*(int value) const;
    double operator*(const Dot &p) const;

    Dot toRGB(int value) const;

    double vector_length() const;

    double vector_angle(const Dot &p) const;

    void scale(double coef, double xCenter, double yCenter, double zCenter);
    void move(double xDif, double yDif, double zDif);
    void rotateX(double angle, double xCenter, double yCenter, double zCenter);
    void rotateY(double angle, double xCenter, double yCenter, double zCenter);
    void rotateZ(double angle, double xCenter, double yCenter, double zCenter);
    void rotateX(double angle, Dot center);
    void rotateY(double angle, Dot center);
    void rotateZ(double angle, Dot center);

    double findDistance(Dot dot);
    double findDistanceToLine(Dot dot1, Dot dot2);

    friend std::ostream &operator<<(std::ostream &os, const Dot &dot);
    friend QDebug operator<<(QDebug deb, const Dot &dot);
};

#endif // DOT_H
