#include "dot.hpp"
#include "QDebug"

double Dot::getX() const { return x; }

void Dot::setX(double value) { x = value; }

double Dot::getY() const { return y; }

void Dot::setY(double value) { y = value; }

double Dot::getZ() const { return z; }

void Dot::setZ(double value) { z = value; }

void Dot::setPos(Dot &pos) { x = pos.getX(); y = pos.getY(), z = pos.getZ(); }

bool Dot::operator==(const Dot &p) const {
  return x == p.x && y == p.y && z == p.z;
}

Dot Dot::operator^(const Dot &p) const {
  Dot res;
  res.x = y * p.z - z * p.y;
  res.y = z * p.x - x * p.z;
  res.z = x * p.y - y * p.x;

  return res;
}

Dot Dot::operator-(const Dot &p) const {
  Dot res;
  res.x = x - p.x;
  res.y = y - p.y;
  res.z = z - p.z;

  return res;
}

double Dot::operator*(const Dot &p) const {
  double res = x * p.x;
  res += y * p.y;
  res += z * p.z;

  return res;
}

Dot Dot::operator*(int value) const {
  Dot res;
  res.x = x * value;
  res.y = y * value;
  res.z = z * value;

  return res;
}

Dot Dot::toRGB(int value) const {
  Dot res;
  res.x = x + value;
  res.y = y + value;
  res.z = z + value;

  if (res.x > 255)
      res.x = 255;
  else if (res.x < 0)
      res.x = 0;

  if (res.y > 255)
      res.y = 255;
  else if (res.y < 0)
      res.y = 0;

  if (res.z > 255)
      res.z = 255;
  else if (res.z < 0)
      res.z = 0;

  return res;
}

double Dot::vector_length() const { return sqrt(x * x + y * y + z * z); }

double Dot::vector_angle(const Dot &p) const {
  double scalar = this->operator*(p);

  return acos((scalar) / (this->vector_length() * p.vector_length()));
}

void Dot::move(double xDif, double yDif, double zDif)
{
    x += xDif;
    y += yDif;
    z += zDif;
}

void Dot::scale(double coef, double xCenter, double yCenter, double zCenter)
{
    x -= xCenter;
    y -= yCenter;
    z -= zCenter;

    Eigen::Matrix4f scaleMatrix;
    // clang-format off
    scaleMatrix << coef, 0, 0, 0,
                   0, coef, 0, 0,
                   0, 0, coef, 0,
                   0, 0,    0, 1;
    // clang-format on
    Eigen::MatrixXf coordinatesVec(1, 4);
    coordinatesVec << x, y, z, 1;

    coordinatesVec = coordinatesVec * scaleMatrix;
    x = coordinatesVec(0, 0) + xCenter;
    y = coordinatesVec(0, 1) + yCenter;
    z = coordinatesVec(0, 2) + zCenter;
}

void Dot::rotateX(double angle, double xCenter, double yCenter, double zCenter)
{
    x -= xCenter;
    y -= yCenter;
    z -= zCenter;

    Eigen::Matrix4f rotateMatrix;
    // clang-format off
    rotateMatrix << 1,           0,          0, 0,
                    0, cos(angle), -sin(angle), 0,
                    0, sin(angle),  cos(angle), 0,
                    0,           0,          0, 1;
    // clang-format on

    Eigen::MatrixXf coordinatesVec(1, 4);
    coordinatesVec << x, y, z, 1;
    coordinatesVec = coordinatesVec * rotateMatrix;
    x = coordinatesVec(0, 0) + xCenter;
    y = coordinatesVec(0, 1) + yCenter;
    z = coordinatesVec(0, 2) + zCenter;
}

void Dot::rotateY(double angle, double xCenter, double yCenter, double zCenter)
{
    x -= xCenter;
    y -= yCenter;
    z -= zCenter;

    Eigen::Matrix4f rotateMatrix;
    // clang-format off
    rotateMatrix << cos(angle),  0, sin(angle), 0,
                    0,           1,          0, 0,
                    -sin(angle), 0, cos(angle), 0,
                    0,           0,          0, 1;
    // clang-format on

    Eigen::MatrixXf coordinatesVec(1, 4);
    coordinatesVec << x, y, z, 1;

    coordinatesVec = coordinatesVec * rotateMatrix;
    x = coordinatesVec(0, 0) + xCenter;
    y = coordinatesVec(0, 1) + yCenter;
    z = coordinatesVec(0, 2) + zCenter;
}

void Dot::rotateZ(double angle, double xCenter, double yCenter, double zCenter)
{
    x -= xCenter;
    y -= yCenter;
    z -= zCenter;

    Eigen::Matrix4f rotateMatrix;
    // clang-format off
    rotateMatrix << cos(angle), -sin(angle), 0, 0,
                    sin(angle),  cos(angle), 0, 0,
                    0,                    0, 1, 0,
                    0,                    0, 0, 1;
    // clang-format on

    Eigen::MatrixXf coordinatesVec(1, 4);
    coordinatesVec << x, y, z, 1;

    coordinatesVec = coordinatesVec * rotateMatrix;
    x = coordinatesVec(0, 0) + xCenter;
    y = coordinatesVec(0, 1) + yCenter;
    z = coordinatesVec(0, 2) + zCenter;
}

void Dot::rotateX(double angle, Dot center)
{
    rotateX(angle, center.getX(), center.getY(), center.getZ());
}

void Dot::rotateY(double angle, Dot center)
{
    rotateY(angle, center.getX(), center.getY(), center.getZ());
}

void Dot::rotateZ(double angle, Dot center)
{
    rotateZ(angle, center.getX(), center.getY(), center.getZ());
}

double Dot::findDistance(Dot dot)
{
    return sqrt(pow(x - dot.x, 2) + pow(y - dot.y, 2) + pow(z - dot.z, 2));
}

double Dot::findDistanceToLine(Dot dot1, Dot dot2)
{
    double dx = dot2.getX() - dot1.getX(),
            dy = dot2.getY() - dot1.getY(),
            dz = dot2.getZ() - dot1.getZ();
    double res = 0;
    if (!dy)
    {
        res = (dz * x - dx * z +
                  dot2.getX() * dot1.getZ() -
                  dot1.getX() * dot2.getZ()) /
               sqrt(pow(dx, 2) + pow(dz, 2));
    }
    else
    {
        //To be continued
        res = 0;
    }
    return res;
}

std::ostream &operator<<(std::ostream &os, const Dot &dot)
{
    os << "Dot3D: (" << dot.x << "; " << dot.y << "; "
       << dot.z << ")\n";
    return os;
}

QDebug operator<<(QDebug deb, const Dot &dot)
{
    deb << "Dot3D: (" << dot.getX() << "; " << dot.getY() << "; "
        << dot.getZ() << ")\n";
    return deb;
}
