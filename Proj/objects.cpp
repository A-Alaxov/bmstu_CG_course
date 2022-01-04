#include "objects.hpp"
#include <QDebug>

const std::vector<Dot> PolModel::getVertices() { return vertices; }

void PolModel::setVertices(std::vector<Dot> &vertices_) { vertices = vertices_; }

const std::vector<Facet> PolModel::getFacets() { return facets; }

void PolModel::changeIntense(size_t fac, double intens) { facets[fac].setIntens(intens); }

void PolModel::setFacets(std::vector<Facet> facets_) { facets = facets_; }

void PolModel::rotateXY(double angle)
{
    double radianAngle = (double) angle * M_PI / 180.0;

    for (size_t i = 0; i < vertices.size(); i++)
    {
        Dot curDot = vertices.at(i);
        curDot.rotateZ(radianAngle, center.getX(), center.getY(), center.getZ());
        vertices.at(i).setPos(curDot);
    }
    collisionCenter.rotateZ(radianAngle, center.getX(), center.getY(), center.getZ());
}

void PolModel::rotateYZ(double angle)
{
    double radianAngle = (double) angle * M_PI / 180.0;

    for (size_t i = 0; i < vertices.size(); i++)
    {
        Dot curDot = vertices.at(i);
        curDot.rotateX(radianAngle, center.getX(), center.getY(), center.getZ());
        vertices.at(i).setPos(curDot);
    }
    collisionCenter.rotateX(radianAngle, center.getX(), center.getY(), center.getZ());
}

void PolModel::rotateZX(double angle)
{
    //double radianAngle = (double) angle * M_PI / 180.0;

    for (size_t i = 0; i < vertices.size(); i++)
    {
        Dot curDot = vertices.at(i);
        curDot.rotateY(angle, center.getX(), center.getY(), center.getZ());
        vertices.at(i).setPos(curDot);
    }
    collisionCenter.rotateY(angle, center.getX(), center.getY(), center.getZ());
    curAngle += angle;
}

void PolModel::move(double xMove, double yMove, double zMove)
{
    for (size_t i = 0; i < vertices.size(); i++)
    {
        Dot curDot = vertices.at(i);
        curDot.move(xMove, yMove, zMove);
        vertices.at(i).setPos(curDot);
    }
    center.move(xMove, yMove, zMove);
    collisionCenter.move(xMove, yMove, zMove);
}

double PolModel::findCollisionAngle(PolModel *md)
{
    double a = sin(md->getAngle()),
            r = radius, h = md->getWidth(), l = SPACE_BETWEEN_DOMINO;
    double x = (2*pow(a, 2)*l - 2*l +
            sqrt(-(4 - 4*pow(a, 2))*(-2*a*h*l + pow(h, 2) + pow(l, 2) - pow(r, 2))
                 + pow(-2*pow(a, 2)*l + 2*l, 2))) /
            (2 - 2*pow(a, 2));
    return acos((pow(r, 2) + pow(l+x, 2) - pow(h+x*a, 2)) / (2*r*(l+x)));
}

Dot PolModel::getCenter() { return center; }

Dot PolModel::getCollisionCenter() { return collisionCenter; }

void PolModel::setCollisionCenter(Dot dot) { collisionCenter = dot; }

void PolModel::setVelocity(double vel, double friction_coef)
{
    velocity = vel;
    setAcceleration(friction_coef);
}

void PolModel::setAcceleration(double friction_coef)
{
    acceleration = mass * friction_coef * G * (velocity > 0 ? -1 : 1);
}

void PolModel::setRadius(double rad)
{
    radius = rad;
    collisionRadius = rad;
}

void PolModel::setCollisionRadius(double rad)
{
    collisionRadius = rad;
}

void PolModel::setMass(double mas)
{
    mass = mas;
    setAcceleration();
}

void PolModel::setAngle(double ang) { curAngle = ang; }

void PolModel::setWidth(double wid) { width = wid; }

double PolModel::getVelocity() { return velocity; }

double PolModel::getAcceleration() { return acceleration; }

double PolModel::getRadius() { return radius; }

double PolModel::getCollisionRadius() { return collisionRadius; }

double PolModel::getMass() { return mass; }

double PolModel::getAngle() { return curAngle; }

double PolModel::getWidth() { return width; }

Dot PolModel::getLeftUp()
{
    int flag = 1;
    double x, z;
    for (auto dot : vertices)
    {
        if (dot.getZ() > z || flag)
        {
            flag = 0;
            z = dot.getZ();
            x = dot.getX();
        }
        else if (dot.getZ() == z)
            x = std::min(x, dot.getX());
    }
    return Dot(x, center.getY(), z);
}

Dot PolModel::getLeftDown()
{
    int flag = 1;
    double x, z;
    for (auto dot : vertices)
    {
        if (dot.getX() < x || flag)
        {
            flag = 0;
            z = dot.getZ();
            x = dot.getX();
        }
        else if (dot.getX() == x)
            z = std::min(z, dot.getZ());
    }
    return Dot(x, center.getY(), z);
}

Dot PolModel::getRightUp()
{
    int flag = 1;
    double x, z;
    for (auto dot : vertices)
    {
        if (dot.getX() > x || flag)
        {
            flag = 0;
            z = dot.getZ();
            x = dot.getX();
        }
        else if (dot.getX() == x)
            z = std::max(z, dot.getZ());
    }
    return Dot(x, center.getY(), z);
}

CellScene::CellScene() {}

CellScene::CellScene(size_t width_, size_t height_)
{
    width = width_;
    height = height_;

    modelsNum = 0;
    illumNum = 0;

    toCenter();
}

void CellScene::addParallelepiped(std::vector<Dot> &vertices, std::vector<Facet> &facets,
    int x1, int y1, int z1, int x2, int y2, int z2)
{
    addQuad(vertices, facets,
            x1, y1, z1,
            x1, y2, z1,
            x2, y2, z1,
            x2, y1, z1);
    addQuad(vertices, facets,
            x1, y1, z1,
            x1, y2, z1,
            x1, y2, z2,
            x1, y1, z2);
    addQuad(vertices, facets,
            x1, y1, z1,
            x1, y1, z2,
            x2, y1, z2,
            x2, y1, z1);
    addQuad(vertices, facets,
            x1, y1, z2,
            x1, y2, z2,
            x2, y2, z2,
            x2, y1, z2);
    addQuad(vertices, facets,
            x2, y1, z1,
            x2, y2, z1,
            x2, y2, z2,
            x2, y1, z2);
    addQuad(vertices, facets,
            x1, y2, z1,
            x1, y2, z2,
            x2, y2, z2,
            x2, y2, z1);
}

void CellScene::addQuad(std::vector<Dot> &vertices, std::vector<Facet> &facets, int x1,
int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4, int y4, int z4)
{
    Dot dot;
    std::vector<size_t> vec;

    size_t size = facets.size();

    dot = Dot(x1, y1, z1);
    vec = {size, size + 1};
    vertices.push_back(Dot(dot));

    dot = Dot(x2, y2, z2);
    vec = {size};
    vertices.push_back(Dot(dot));

    dot = Dot(x3, y3, z3);
    vec = {size, size + 1};
    vertices.push_back(Dot(dot));

    dot = Dot(x4, y4, z4);
    vec = {size + 1};
    vertices.push_back(Dot(dot));

    size = vertices.size();
    vec = {size - 4, size - 3, size - 2};
    facets.emplace_back(vec, BROWN);
    vec = {size - 4, size - 2, size - 1};
    facets.emplace_back(vec, BROWN);
}

size_t CellScene::getWidth() { return width; }

size_t CellScene::getHeight() { return height; }

CellScene::operator bool() const { return this->plateModel; }

void CellScene::moveUp(double value)
{
    Eigen::Matrix4f moveMat;
    moveMat << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, -value, 0, 1;

    transMatrix *= moveMat;
}

void CellScene::moveDown(double value)
{
    Eigen::Matrix4f moveMat;
    moveMat << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, value, 0, 1;

    transMatrix *= moveMat;
}

void CellScene::moveLeft(double value)
{
    Eigen::Matrix4f moveMat;
    moveMat << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -value, 0, 0, 1;

    transMatrix *= moveMat;
}

void CellScene::moveRight(double value)
{
    Eigen::Matrix4f moveMat;
    moveMat << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, value, 0, 0, 1;

    transMatrix *= moveMat;
}

void CellScene::scale(double value)
{
    Eigen::Matrix4f scaleMatrix;
    // clang-format off
    scaleMatrix << value,     0,     0, 0,
                    0,    value,     0, 0,
                    0,        0, value, 0,
                    0,        0,     0, 1;
    // clang-format on
    transMatrix *= scaleMatrix;
}

void CellScene::rotateX(double angle)
{
    Eigen::Matrix4f rotateMatrix;
    // clang-format off
    rotateMatrix << 1,           0,          0, 0,
                    0, cos(angle), -sin(angle), 0,
                    0, sin(angle),  cos(angle), 0,
                    0,           0,          0, 1;
    // clang-format on

    transMatrix *= rotateMatrix;
}

void CellScene::rotateY(double angle)
{
    Eigen::Matrix4f rotateMatrix;
    // clang-format off
    rotateMatrix << cos(angle),  0, sin(angle), 0,
                    0,           1,          0, 0,
                    -sin(angle), 0, cos(angle), 0,
                    0,           0,          0, 1;
    // clang-format on

    transMatrix *= rotateMatrix;
}

void CellScene::rotateZ(double angle)
{
    Eigen::Matrix4f rotateMatrix;
    // clang-format off
    rotateMatrix << cos(angle),  -sin(angle), 0, 0,
                    sin(angle),   cos(angle), 0, 0,
                    0,            0,          1, 0,
                    0,            0,          0, 1;
    // clang-format on

    transMatrix *= rotateMatrix;
}

void CellScene::toCenter()
{
    Dot start(PLATE_START);

    Eigen::Matrix4f newMat;
    // clang-format off
    newMat << 1,  0, 0, 0,
              0,  1, 0, 0,
              0,  0, 1, 0,
              0,  0, 0, 1;
    // clang-format on
    newMat(3, 0) = X_CENTER - start.getX() - getWidth() * SCALE_FACTOR / 2;
    newMat(3, 1) = Y_CENTER - start.getY() - getHeight() * SCALE_FACTOR / 2;
    newMat(3, 2) = 0;

    transMatrix = newMat;
}

size_t CellScene::getModelsNum() { return modelsNum; }

PolModel &CellScene::getModel(size_t num) { return models.at(num); }

PolModel *CellScene::getCoolModel(size_t num) { return &models[num]; }

void CellScene::setModel(size_t num, PolModel &newModel) { models.at(num) = newModel; }

void CellScene::addModel(PolModel &model)
{
    modelsNum++;
    models.push_back(model);
}

size_t CellScene::getIllumNum() { return illumNum; }

Illuminant &CellScene::getIlluminant(size_t num) { return illuminants.at(num); }

void CellScene::setIlluminant(Illuminant &illum, size_t i) { illuminants.at(i) = illum; }

void CellScene::addIlluminant(Illuminant &illum)
{
    illumNum++;
    illuminants.push_back(illum);
}

void CellScene::deleteIlluminant(size_t num)
{
    if (num < illuminants.size())
    {
        illumNum--;
        illuminants.erase(illuminants.begin() + num);
    }
}

void CellScene::deleteModel(size_t num)
{
    if (num < models.size())
    {
        modelsNum--;
        models.erase(models.begin() + num);
    }
}

PolModel &CellScene::getPlateModel() { return *plateModel; }

PolModel *CellScene::getCoolPlateModel() { return plateModel; }

void CellScene::buildPlateModel(Dot startOfPlate_, Dot endOfPlate_)
{
    std::vector<Dot> vertices;
    std::vector<Facet> facets;

    addQuad(vertices, facets, startOfPlate_.getX(),
    startOfPlate_.getY(), PLATE_Z - 10, startOfPlate_.getX(),
    endOfPlate_.getY() + 10, PLATE_Z - 10, endOfPlate_.getX() + 10,
    endOfPlate_.getY() + 10, PLATE_Z - 10, endOfPlate_.getX() + 10,
    startOfPlate_.getY(), PLATE_Z - 10);

    addQuad(vertices, facets, startOfPlate_.getX(),
    startOfPlate_.getY(), PLATE_Z, startOfPlate_.getX(),
    endOfPlate_.getY() + 10, PLATE_Z, endOfPlate_.getX() + 10,
    endOfPlate_.getY() + 10, PLATE_Z, endOfPlate_.getX() + 10,
    startOfPlate_.getY(), PLATE_Z);

    addQuad(vertices, facets, startOfPlate_.getX(),
    startOfPlate_.getY(), PLATE_Z - 10, startOfPlate_.getX(),
    endOfPlate_.getY() + 10, PLATE_Z - 10, startOfPlate_.getX(),
    endOfPlate_.getY() + 10, PLATE_Z, startOfPlate_.getX(),
    startOfPlate_.getY(), PLATE_Z);

    addQuad(vertices, facets, startOfPlate_.getX(),
    endOfPlate_.getY() + 10, PLATE_Z - 10, endOfPlate_.getX() + 10,
    endOfPlate_.getY() + 10, PLATE_Z - 10, endOfPlate_.getX() + 10,
    endOfPlate_.getY() + 10, PLATE_Z, startOfPlate_.getX(),
    endOfPlate_.getY() + 10, PLATE_Z);

    addQuad(vertices, facets, endOfPlate_.getX() + 10,
    endOfPlate_.getY() + 10, PLATE_Z - 10, endOfPlate_.getX() + 10,
    startOfPlate_.getY(), PLATE_Z - 10, endOfPlate_.getX() + 10,
    startOfPlate_.getY(), PLATE_Z, endOfPlate_.getX() + 10,
    endOfPlate_.getY() + 10, PLATE_Z);

    addQuad(vertices, facets, endOfPlate_.getX() + 10,
    startOfPlate_.getY(), PLATE_Z - 10, startOfPlate_.getX(),
    startOfPlate_.getY(), PLATE_Z - 10, startOfPlate_.getX(),
    startOfPlate_.getY(), PLATE_Z, endOfPlate_.getX() + 10,
    startOfPlate_.getY(), PLATE_Z);

    // Legs
    addParallelepiped(vertices, facets,
    startOfPlate_.getX(), startOfPlate_.getY(), PLATE_Z - 10,
    startOfPlate_.getX() + 50, startOfPlate_.getY() + 50, PLATE_Z - 510);
    addParallelepiped(vertices, facets,
    endOfPlate_.getX() + 10, endOfPlate_.getY() + 10, PLATE_Z - 10,
    endOfPlate_.getX() + 10 - 50, endOfPlate_.getY() + 10 - 50, PLATE_Z - 510);
    addParallelepiped(vertices, facets,
    startOfPlate_.getX(), endOfPlate_.getY() + 10, PLATE_Z - 10,
    startOfPlate_.getX() + 50, endOfPlate_.getY() + 10 - 50, PLATE_Z - 510);
    addParallelepiped(vertices, facets,
    endOfPlate_.getX() + 10, startOfPlate_.getY(), PLATE_Z - 10,
    endOfPlate_.getX() + 10 - 50, startOfPlate_.getY() + 50, PLATE_Z - 510);
    if (plateModel)
        delete plateModel;
    plateModel = new PolModel(vertices, facets);
}

void CellScene::setObjects()
{
    models[0].move(250, 0, 0);
    models[0].setMass(2);
    models[0].setAngle(0);

    models[3].move(350, 150, 0);
    models[4].move(525 + SPACE_BETWEEN_DOMINO * 10.5, 300, 0);

    models[2].move(400 + SPACE_BETWEEN_DOMINO * 10.5, 0, 0);
    models[2].rotateZX(M_PI_2);
    models[2].setAcceleration(MU_BUTTON);

    for (size_t i = DOMINO_START; i < modelsNum; i++)
    {
        models[i].move(400 + SPACE_BETWEEN_DOMINO * (i - DOMINO_START), 0, 0);
        models[i].setAngle(M_PI_2);
    }
}

Eigen::Matrix4f &CellScene::getTransMatrix() { return transMatrix; }
