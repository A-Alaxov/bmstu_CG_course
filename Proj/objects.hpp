#ifndef OBJS_HPP
#define OBJS_HPP

#include "surface.hpp"
#include "illuminant.hpp"
#include <iostream>

#include "config.hpp"

class PolModel
{
public:
    PolModel() {}
    PolModel(std::vector<Dot> vertices_, std::vector<Facet> facets_)
    : vertices(vertices_), facets(facets_)
    {
    }

    PolModel(std::vector<Dot> vertices_, std::vector<Facet> facets_, QString name_, Dot center)
    : vertices(vertices_), facets(facets_), modelName(name_), center(center), collisionCenter(center), velocity(0), acceleration(0)
    {
    }

    PolModel(std::vector<Dot> vertices_, std::vector<Facet> facets_, QString name_)
        : vertices(vertices_), facets(facets_), modelName(name_)
    {
    }

    ~PolModel() {}

    const std::vector<Dot> getVertices();
    void setVertices(std::vector<Dot> &vertices_);
    const std::vector<Facet> getFacets();
    void changeIntense(size_t fac, double intens);
    void setFacets(std::vector<Facet> facets_);

    void rotateXY(double angle);
    void rotateYZ(double angle);
    void rotateZX(double angle);

    void move(double xMove, double yMove, double zCell);

    double findCollisionAngle(PolModel *md);

    Dot getCenter();
    Dot getCollisionCenter();
    void setCollisionCenter(Dot dot);

    void setVelocity(double vel, double friction_coef = MU);
    void setAcceleration(double friction_coef = MU);
    void setRadius(double rad);
    void setCollisionRadius(double rad);
    void setMass(double mas);
    void setAngle(double ang);
    void setWidth(double wid);

    double getVelocity();
    double getAcceleration();
    double getRadius();
    double getCollisionRadius();
    double getMass();
    double getAngle();
    double getWidth();

    Dot getLeftUp();
    Dot getLeftDown();
    Dot getRightUp();

private:
    std::vector<Dot> vertices;
    std::vector<Facet> facets;
    QString modelName;
    Dot collisionCenter;
    Dot center;
    double velocity;
    double acceleration;
    double radius;
    double collisionRadius;
    double mass;
    double curAngle;
    double width;
};

class CellScene
{
public:
    CellScene();
    CellScene(size_t width_, size_t height_);

    operator bool() const;

    void setObjects();

    size_t getWidth();
    size_t getHeight();

    void buildPlateModel(Dot startOfPlate_, Dot endOfPlate_);
    PolModel &getPlateModel();
    PolModel *getCoolPlateModel();

    void moveUp(double value);
    void moveDown(double value);
    void moveLeft(double value);
    void moveRight(double value);

    void scale(double value);

    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);

    void toCenter();

    size_t getModelsNum();
    PolModel &getModel(size_t num);
    PolModel *getCoolModel(size_t num);
    void setModel(size_t num, PolModel &newModel);
    void addModel(PolModel &model);
    void deleteModel(size_t num);

    size_t getIllumNum();
    Illuminant &getIlluminant(size_t num);
    void setIlluminant(Illuminant &illum, size_t i);
    void addIlluminant(Illuminant &illum);
    void deleteIlluminant(size_t num);

    Eigen::Matrix4f &getTransMatrix();
    void multToTrans(Eigen::Matrix4f &newTrans);

private:
    void addParallelepiped(std::vector<Dot> &vertices, std::vector<Facet> &facets,
        int x1, int y1, int z1, int x2, int y2, int z2);
    void addQuad(std::vector<Dot> &vertices, std::vector<Facet> &facets, int x1,
    int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4, int y4,
    int z4);

    size_t width, height;

    PolModel *plateModel = nullptr;

    Eigen::Matrix4f transMatrix;

    size_t modelsNum = 0;
    std::vector<PolModel> models;

    size_t illumNum = 0;
    std::vector<Illuminant> illuminants;

    Dot center;
};

#endif // OBJS_HPP
