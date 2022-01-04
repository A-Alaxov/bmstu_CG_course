#include "usagefacade.hpp"

#include "QDebug"
#include "QPen"

#include <QCoreApplication>
#include <limits>
#include <QDebug>
#include "math.h"
#include <QTime>

UsageFacade::UsageFacade() { drawer = new Drawer; }

void UsageFacade::setCellScene(size_t width_, size_t height_)
{
    if (scene)
        delete scene;
    scene = new CellScene(width_, height_);
    qDebug("Set was done\n");
}

bool UsageFacade::isSceneSet() { return scene; }

QGraphicsScene *UsageFacade::drawScene(QRectF rect, nanoseconds *cur)
{
    QGraphicsScene *retScene = nullptr;
    if (isSceneSet())
        retScene = drawer->drawScene(scene, rect, cur);

    return retScene;
}

QGraphicsScene *UsageFacade::moveUpScene(double value, QRectF rect)
{
    scene->moveUp(value);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::moveDownScene(double value, QRectF rect)
{
    scene->moveDown(value);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::moveRightScene(double value, QRectF rect)
{
    scene->moveRight(value);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::moveLeftScene(double value, QRectF rect)
{
    scene->moveLeft(value);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::scaleScene(double value, QRectF rect)
{
    scene->scale(value);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::rotateXScene(double angle, QRectF rect)
{
    scene->rotateX(angle);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::rotateYScene(double angle, QRectF rect)
{
    scene->rotateY(angle);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::rotateZScene(double angle, QRectF rect)
{
    scene->rotateZ(angle);

    return drawScene(rect);
}

CellScene *UsageFacade::getScene() { return scene; }

void UsageFacade::addParallelepiped(std::vector<Dot> &vertices, std::vector<Facet> &facets,
    int x1, int y1, int z1, int x2, int y2, int z2, size_t color)
{
    addQuad(vertices, facets,
            x1, y1, z1,
            x1, y2, z1,
            x2, y2, z1,
            x2, y1, z1, color);
    addQuad(vertices, facets,
            x1, y1, z1,
            x1, y2, z1,
            x1, y2, z2,
            x1, y1, z2, color);
    addQuad(vertices, facets,
            x1, y1, z1,
            x1, y1, z2,
            x2, y1, z2,
            x2, y1, z1, color);
    addQuad(vertices, facets,
            x1, y1, z2,
            x1, y2, z2,
            x2, y2, z2,
            x2, y1, z2, color);
    addQuad(vertices, facets,
            x2, y1, z1,
            x2, y2, z1,
            x2, y2, z2,
            x2, y1, z2, color);
    addQuad(vertices, facets,
            x1, y2, z1,
            x1, y2, z2,
            x2, y2, z2,
            x2, y2, z1, color);
}

void UsageFacade::addQuad(std::vector<Dot> &vertices, std::vector<Facet> &facets,
    int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4,
    int y4, int z4, size_t color)
{
    Dot dot;
    bool f;
    std::vector<size_t> vec1, vec2;

    size_t size = facets.size();

    f = 0;
    dot = Dot(x1, y1, z1);
    for (size_t i = 0; i < vertices.size() && !f; i++)
        if (vertices[i] == dot)
        {
            f = 1;
            vec1.push_back(i);
            vec2.push_back(i);
            dot = Dot(vertices[i]);
            dot.addFacet(size);
            dot.addFacet(size + 1);
            vertices[i] = dot;
        }
    if (!f)
    {
        vec1.push_back(vertices.size());
        vec2.push_back(vertices.size());
        dot.addFacet(size);
        dot.addFacet(size + 1);
        vertices.push_back(Dot(dot));
    }

    f = 0;
    dot = Dot(x2, y2, z2);
    for (size_t i = 0; i < vertices.size() && !f; i++)
        if (vertices[i] == dot)
        {
            f = 1;
            vec1.push_back(i);
            dot = Dot(vertices[i]);
            dot.addFacet(size);
            vertices[i] = dot;
        }
    if (!f)
    {
        vec1.push_back(vertices.size());
        dot.addFacet(size);
        vertices.push_back(Dot(dot));
    }

    f = 0;
    dot = Dot(x3, y3, z3);
    for (size_t i = 0; i < vertices.size() && !f; i++)
        if (vertices[i] == dot)
        {
            f = 1;
            vec1.push_back(i);
            vec2.push_back(i);
            dot = Dot(vertices[i]);
            dot.addFacet(size);
            dot.addFacet(size + 1);
            vertices[i] = dot;
        }
    if (!f)
    {
        vec1.push_back(vertices.size());
        vec2.push_back(vertices.size());
        dot.addFacet(size);
        dot.addFacet(size + 1);
        vertices.push_back(Dot(dot));
    }

    f = 0;
    dot = Dot(x4, y4, z4);
    for (size_t i = 0; i < vertices.size() && !f; i++)
        if (vertices[i] == dot)
        {
            f = 1;
            vec2.push_back(i);
            dot = Dot(vertices[i]);
            dot.addFacet(size + 1);
            vertices[i] = dot;
        }
    if (!f)
    {
        vec2.push_back(vertices.size());
        dot.addFacet(size + 1);
        vertices.push_back(Dot(dot));
    }

    size = vertices.size();
    facets.emplace_back(vec1, color, std::vector<bool>({true, true, true}));
    facets.emplace_back(vec2, color, std::vector<bool>({true, true, true}));
}

void UsageFacade::addTriangle(std::vector<Dot> &vertices, std::vector<Facet> &facets,
    int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, size_t color)
{
    Dot dot;
    bool f;
    std::vector<size_t> vec;

    size_t size = facets.size();

    f = 0;
    dot = Dot(x1, y1, z1);
    for (size_t i = 0; i < vertices.size() && !f; i++)
        if (vertices[i] == dot)
        {
            f = 1;
            vec.push_back(i);
            dot = Dot(vertices[i]);
            dot.addFacet(size);
            vertices[i] = dot;
        }
    if (!f)
    {
        vec.push_back(vertices.size());
        dot.addFacet(size);
        vertices.push_back(Dot(dot));
    }

    f = 0;
    dot = Dot(x2, y2, z2);
    for (size_t i = 0; i < vertices.size() && !f; i++)
        if (vertices[i] == dot)
        {
            f = 1;
            vec.push_back(i);
            dot = Dot(vertices[i]);
            dot.addFacet(size);
            vertices[i] = dot;
        }
    if (!f)
    {
        vec.push_back(vertices.size());
        dot.addFacet(size);
        vertices.push_back(Dot(dot));
    }

    f = 0;
    dot = Dot(x3, y3, z3);
    for (size_t i = 0; i < vertices.size() && !f; i++)
        if (vertices[i] == dot)
        {
            f = 1;
            vec.push_back(i);
            dot = Dot(vertices[i]);
            dot.addFacet(size);
            vertices[i] = dot;
        }
    if (!f)
    {
        vec.push_back(vertices.size());
        dot.addFacet(size);
        vertices.push_back(Dot(dot));
    }

    facets.emplace_back(vec, color);
}

int UsageFacade::addCylindah()
{
    std::vector<Dot> vertices;
    std::vector<Facet> facets;

    model_file_loader mfl;
    std::string fname = "cylindah.txt";
    mfl.open(fname);
    Dot center = mfl.read_center();
    std::vector<Dot> dots = mfl.read_points();
    std::vector<surface> edges = mfl.read_edges();

    for (auto edge : edges) {
        addQuad(vertices, facets,
                dots[edge.get_first()].getX(), dots[edge.get_first()].getY(), dots[edge.get_first()].getZ(),
                dots[edge.get_second()].getX(), dots[edge.get_second()].getY(), dots[edge.get_second()].getZ(),
                dots[edge.get_third()].getX(), dots[edge.get_third()].getY(), dots[edge.get_third()].getZ(),
                dots[(edge.get_third() + 1) % (dots.size() / 2)].getX(),
                dots[(edge.get_third() + 1) % (dots.size() / 2)].getY(),
                dots[(edge.get_third() + 1) % (dots.size() / 2)].getZ(),
                RED);
        Facet tmp = facets[facets.size() - 1];
        tmp.setGuro();
        facets[facets.size() - 1] = tmp;
        tmp = facets[facets.size() - 2];
        tmp.setGuro();
        facets[facets.size() - 2] = tmp;
    }

    for (size_t i = 0; i < dots.size() / 4; i++) {
        addQuad(vertices, facets,
                dots[i].getX(), dots[i].getY(), dots[i].getZ(),
                dots[i + 1].getX(), dots[i + 1].getY(), dots[i + 1].getZ(),
                dots[(i + 1 + dots.size() / 4) % (dots.size() / 2)].getX(),
                dots[(i + 1 + dots.size() / 4) % (dots.size() / 2)].getY(),
                dots[(i + 1 + dots.size() / 4) % (dots.size() / 2)].getZ(),
                dots[i + dots.size() / 4].getX(), dots[i + dots.size() / 4].getY(), dots[i + dots.size() / 4].getZ(),
                RED);
    }

    for (size_t i = dots.size() / 2; i < 3 * dots.size() / 4; i++) {
        addQuad(vertices, facets,
                dots[i].getX(), dots[i].getY(), dots[i].getZ(),
                dots[i + 1].getX(), dots[i + 1].getY(), dots[i + 1].getZ(),
                dots[dots.size() / 2 + (i + 1 + dots.size() / 4) % (dots.size() / 2)].getX(),
                dots[dots.size() / 2 + (i + 1 + dots.size() / 4) % (dots.size() / 2)].getY(),
                dots[dots.size() / 2 + (i + 1 + dots.size() / 4) % (dots.size() / 2)].getZ(),
                dots[i + dots.size() / 4].getX(), dots[i + dots.size() / 4].getY(), dots[i + dots.size() / 4].getZ(),
                RED);
    }

    PolModel cylindahModel(vertices, facets, "Cylindah", center);
    cylindahModel.setRadius(25);
    cylindahModel.setMass(3.1);

    scene->addModel(cylindahModel);

    return 0;
}

int UsageFacade::addButtonStand()
{
    std::vector<Dot> vertices;
    std::vector<Facet> facets;

    addParallelepiped(vertices, facets,
                      0, -38, PLATE_Z,
                      75, 37, PLATE_Z + 100, GRAY);

    PolModel buttonStandModel(vertices, facets, "Button stand");

    scene->addModel(buttonStandModel);

    return 0;
}

int UsageFacade::addStand()
{
    std::vector<Dot> vertices;
    std::vector<Facet> facets;

    addParallelepiped(vertices, facets,
                      -100, -100, PLATE_Z,
                      100, 100, PLATE_Z + 20, YELLOW);

    addParallelepiped(vertices, facets,
                      -20, -80, PLATE_Z + 20,
                      20, -60, PLATE_Z + 470, SILVER);

    addParallelepiped(vertices, facets,
                      -5, -60, PLATE_Z + 450,
                      5, 170, PLATE_Z + 460, SILVER);

    PolModel standModel(vertices, facets, "Stand");

    scene->addModel(standModel);

    return 0;
}

int UsageFacade::addDomino()
{
    std::vector<Dot> vertices;
    std::vector<Facet> facets;

    model_file_loader mfl;
    std::string fname = "domino.txt";
    mfl.open(fname);
    mfl.read_center();
    Dot center = mfl.read_collision_center();
    std::vector<Dot> dots = mfl.read_points();
    std::vector<surface> edges = mfl.read_edges();

    addQuad(vertices, facets,
            dots[0].getX(), dots[0].getY(), dots[0].getZ(),
            dots[1].getX(), dots[1].getY(), dots[1].getZ(),
            dots[2].getX(), dots[2].getY(), dots[2].getZ(),
            dots[3].getX(), dots[3].getY(), dots[3].getZ(),
            BLACK);
    addQuad(vertices, facets,
            dots[8].getX(), dots[8].getY(), dots[8].getZ(),
            dots[9].getX(), dots[9].getY(), dots[9].getZ(),
            dots[10].getX(), dots[10].getY(), dots[10].getZ(),
            dots[11].getX(), dots[11].getY(), dots[11].getZ(),
            BLACK);

    addQuad(vertices, facets,
            dots[5].getX(), dots[5].getY(), dots[5].getZ(),
            dots[4].getX(), dots[4].getY(), dots[4].getZ(),
            dots[8].getX(), dots[8].getY(), dots[8].getZ(),
            dots[9].getX(), dots[9].getY(), dots[9].getZ(),
            BLACK);
    addQuad(vertices, facets,
            dots[6].getX(), dots[6].getY(), dots[6].getZ(),
            dots[5].getX(), dots[5].getY(), dots[5].getZ(),
            dots[9].getX(), dots[9].getY(), dots[9].getZ(),
            dots[10].getX(), dots[10].getY(), dots[10].getZ(),
            BLACK);
    addQuad(vertices, facets,
            dots[7].getX(), dots[7].getY(), dots[7].getZ(),
            dots[6].getX(), dots[6].getY(), dots[6].getZ(),
            dots[10].getX(), dots[10].getY(), dots[10].getZ(),
            dots[11].getX(), dots[11].getY(), dots[11].getZ(),
            BLACK);
    addQuad(vertices, facets,
            dots[4].getX(), dots[4].getY(), dots[4].getZ(),
            dots[7].getX(), dots[7].getY(), dots[7].getZ(),
            dots[11].getX(), dots[11].getY(), dots[11].getZ(),
            dots[8].getX(), dots[8].getY(), dots[8].getZ(),
            BLACK);

    addQuad(vertices, facets,
            dots[1].getX(), dots[1].getY(), dots[1].getZ(),
            dots[0].getX(), dots[0].getY(), dots[0].getZ(),
            dots[4].getX(), dots[4].getY(), dots[4].getZ(),
            dots[5].getX(), dots[5].getY(), dots[5].getZ(),
            BLACK);
    addQuad(vertices, facets,
            dots[2].getX(), dots[2].getY(), dots[2].getZ(),
            dots[1].getX(), dots[1].getY(), dots[1].getZ(),
            dots[5].getX(), dots[5].getY(), dots[5].getZ(),
            dots[6].getX(), dots[6].getY(), dots[6].getZ(),
            BLACK);
    addQuad(vertices, facets,
            dots[3].getX(), dots[3].getY(), dots[3].getZ(),
            dots[2].getX(), dots[2].getY(), dots[2].getZ(),
            dots[6].getX(), dots[6].getY(), dots[6].getZ(),
            dots[7].getX(), dots[7].getY(), dots[7].getZ(),
            BLACK);
    addQuad(vertices, facets,
            dots[0].getX(), dots[0].getY(), dots[0].getZ(),
            dots[3].getX(), dots[3].getY(), dots[3].getZ(),
            dots[7].getX(), dots[7].getY(), dots[7].getZ(),
            dots[4].getX(), dots[4].getY(), dots[4].getZ(),
            BLACK);

    for (size_t i = 12; i < dots.size(); i++) {
        addQuad(vertices, facets,
                dots[i].getX(), dots[i].getY() - 5, dots[i].getZ() - 5,
                dots[i].getX(), dots[i].getY() - 5, dots[i].getZ() + 5,
                dots[i].getX(), dots[i].getY() + 5, dots[i].getZ() + 5,
                dots[i].getX(), dots[i].getY() + 5, dots[i].getZ() - 5,
                WHITE);
    }

    PolModel dominoModel(vertices, facets, "Domino", center);

    dominoModel.setWidth(dots[1].getX() - dots[0].getX());
    dominoModel.setRadius(100);
    dominoModel.setMass(1);

    scene->addModel(dominoModel);

    return 0;
}

int UsageFacade::addPendulum()
{
    std::vector<Dot> vertices;
    std::vector<Facet> facets;

    model_file_loader mfl;
    //std::string fname = "pyramid_10.txt";
    mfl.open(fname);
    Dot center = mfl.read_center();
    std::vector<Dot> dots = mfl.read_points();
    std::vector<surface> edges = mfl.read_edges();

    for (auto edge : edges) {
        addTriangle(vertices, facets,
                    dots[edge.get_first()].getX(), dots[edge.get_first()].getY(), dots[edge.get_first()].getZ(),
                    dots[edge.get_second()].getX(), dots[edge.get_second()].getY(), dots[edge.get_second()].getZ(),
                    dots[edge.get_third()].getX(), dots[edge.get_third()].getY(), dots[edge.get_third()].getZ(), BLUE);
        Facet tmp = facets[facets.size() - 1];
        tmp.setGuro();
        facets[facets.size() - 1] = tmp;
    }
    double cenX = center.getX(), cenY = center.getY(), cenZ = center.getZ();
    addQuad(vertices, facets,
            cenX - 1, cenY - 1, cenZ,
            cenX - 1, cenY + 1, cenZ,
            cenX + 1, cenY + 1, cenZ,
            cenX + 1, cenY - 1, cenZ,
            BLACK);
    addQuad(vertices, facets,
            cenX - 1, cenY - 1, cenZ + 400,
            cenX - 1, cenY + 1, cenZ + 400,
            cenX + 1, cenY + 1, cenZ + 400,
            cenX + 1, cenY - 1, cenZ + 400,
            BLACK);
    addQuad(vertices, facets,
            cenX - 1, cenY + 1, cenZ,
            cenX - 1, cenY - 1, cenZ,
            cenX - 1, cenY - 1, cenZ + 400,
            cenX - 1, cenY + 1, cenZ + 400,
            BLACK);
    addQuad(vertices, facets,
            cenX + 1, cenY + 1, cenZ,
            cenX - 1, cenY + 1, cenZ,
            cenX - 1, cenY + 1, cenZ + 400,
            cenX + 1, cenY + 1, cenZ + 400,
            BLACK);
    addQuad(vertices, facets,
            cenX + 1, cenY - 1, cenZ,
            cenX + 1, cenY + 1, cenZ,
            cenX + 1, cenY + 1, cenZ + 400,
            cenX + 1, cenY - 1, cenZ + 400,
            BLACK);
    addQuad(vertices, facets,
            cenX - 1, cenY - 1, cenZ,
            cenX + 1, cenY - 1, cenZ,
            cenX + 1, cenY - 1, cenZ + 400,
            cenX - 1, cenY - 1, cenZ + 400,
            BLACK);

    center.setZ(cenZ + 400);

    PolModel pendulumModel(vertices, facets, "Pendulum", center);

    center.setZ(cenZ);
    pendulumModel.setCollisionCenter(center);
    pendulumModel.setRadius(400);
    pendulumModel.setCollisionRadius(50);
    pendulumModel.setMass(1);

    scene->addModel(pendulumModel);

    return 0;
}

int UsageFacade::addSphere()
{
    std::vector<Dot> vertices;
    std::vector<Facet> facets;

    model_file_loader mfl;
    //std::string fname = "pyramid_10.txt";
    mfl.open(fname);
    Dot center = mfl.read_center();
    std::vector<Dot> dots = mfl.read_points();
    std::vector<surface> edges = mfl.read_edges();

    for (auto edge : edges) {
        addTriangle(vertices, facets,
                    dots[edge.get_first()].getX(), dots[edge.get_first()].getY(), dots[edge.get_first()].getZ(),
                    dots[edge.get_second()].getX(), dots[edge.get_second()].getY(), dots[edge.get_second()].getZ(),
                    dots[edge.get_third()].getX(), dots[edge.get_third()].getY(), dots[edge.get_third()].getZ());
        Facet tmp = facets[facets.size() - 1];
        tmp.setGuro();
        facets[facets.size() - 1] = tmp;
    }

    PolModel sphereModel(vertices, facets, "Sphere", center);
    sphereModel.setRadius(50);
    sphereModel.setMass(1);

    scene->addModel(sphereModel);

    return 0;
}

void UsageFacade::addIlluminant(int xAngle, int yAngle)
{
    Eigen::Matrix4f transMat;
    double radianXAngle = (double) xAngle * M_PI / 180.0;
    double radianYAngle = (double) yAngle * M_PI / 180.0;

    transMat << 1,           0,          0, 0,
               0, cos(radianXAngle), -sin(radianXAngle), 0,
               0, sin(radianXAngle),  cos(radianXAngle), 0,
               0,           0,           0, 1;

    Eigen::Matrix4f prodMat;
    prodMat << cos(radianYAngle), 0, sin(radianYAngle), 0,
                0, 1, 0, 0,
                -sin(radianYAngle), 0, cos(radianYAngle), 0,
                0, 0, 0, 1;

    transMat *= prodMat;

    Illuminant illum(transMat);
    illum.setAngles(xAngle, yAngle);
    scene->addIlluminant(illum);
}

void UsageFacade::setObjects()
{
    scene->setObjects();
}

void UsageFacade::modelling(SpecialGraphicsView *graphicsView)
{
    isModelling = 1;
    PolModel *model1 = &(scene->getModel(1)),
            *model2 = &(scene->getModel(0)),
            *model3 = &(scene->getModel(2));
    std::vector<PolModel*> dominos;
    std::vector<double> dominoV0;
    std::vector<int> dominoMoving, dominoEnded;
    for (size_t i = DOMINO_START; i < scene->getModelsNum(); i++)
    {
        dominos.push_back(&(scene->getModel(i)));
        dominoV0.push_back(0);
        dominoMoving.push_back(0);
        dominoEnded.push_back(0);
    }

    int buttonEnded = 0;
    double m2_speed_vect = 1, v, new_v, old_v = 0;
    double m2_v0 = 0;
    double fin_angle, collision_angle, new_angle, collAngle;

    model1->setVelocity(40);

    while (!buttonEnded){
        // Sphere
        if (model1->getVelocity())
        {
            v = model1->getVelocity();
            model1->move(v, 0, 0);
            model1->rotateZX(-v / model1->getRadius());

            new_v = v + model1->getAcceleration();
            model1->setVelocity(new_v * v < 0 ? 0 : new_v);
        }
        // Mayatnik
        v = model2->getVelocity();
        if (v)
            model2->rotateZX(v / model2->getRadius());
        if (m2_v0)
        {
            new_v = pow(m2_v0, 2) - 2 * G * model2->getRadius() * (1 - cos(model2->getAngle()));
            if (new_v < 0)
                m2_speed_vect *= -1;
            new_v = sqrt(abs(new_v));
            model2->setVelocity(new_v * m2_speed_vect);
        }
        // Button
        if (model3->getVelocity())
        {
            v = model3->getVelocity();
            new_v = v + model3->getAcceleration();
            model3->setVelocity(new_v * v < 0 ? 0 : new_v, MU_BUTTON);
            v = model3->getVelocity();
            model3->move(v, 0, 0);
            if (!v)
                buttonEnded = 1;
        }
        // Domino
        v = dominos[9]->getVelocity();
        if (v)
            dominos[9]->rotateZX(-v / dominos[9]->getRadius());
        if (dominoV0[9])
        {
            new_v = sqrt(abs(pow(dominoV0[9], 2) + 2 * G * dominos[9]->getRadius() * (1 - sin(dominos[9]->getAngle()))));
            if (dominos[9]->getAngle() - new_v / dominos[9]->getRadius() < 0)
            {
                dominoV0[9] = 0;
                dominoMoving[9] = 0;
                dominoEnded[9] = 1;
                dominos[9]->rotateZX(-dominos[9]->getAngle());
                new_v = 0;
            }
            dominos[9]->setVelocity(new_v);
        }
        // Domino
        for (int i = 8; i > -1; i--)
        {
            v = dominos[i]->getVelocity();
            if (v > 0)
                dominos[i]->rotateZX(-v / dominos[i]->getRadius());
            if (dominoMoving[i])
            {
                new_v = sqrt(abs(pow(dominoV0[i], 2) + 2 * G * dominos[i]->getRadius() * (1 - sin(dominos[i]->getAngle()))));
                fin_angle = atan(dominos[i + 1]->getWidth() / SPACE_BETWEEN_DOMINO);
                collision_angle = acos((SPACE_BETWEEN_DOMINO - dominos[i + 1]->getWidth()) / dominos[i]->getRadius());
                new_angle = dominos[i]->getAngle() - new_v / dominos[i]->getRadius();
                collAngle = dominos[i]->findCollisionAngle(dominos[i + 1]);

                Dot rotatedRightUp = dominos[i]->getRightUp();
                rotatedRightUp.rotateY(new_angle, dominos[i]->getCenter());

                if (!dominoMoving[i + 1] && !dominoEnded[i + 1] && new_angle < collision_angle)
                {
                    dominos[i]->rotateZX(-dominos[i]->getAngle() + collision_angle);
                }
                else if (!dominoEnded[i + 1] && new_angle < collAngle)
                {
                    dominos[i]->rotateZX(-dominos[i]->getAngle() + collAngle);
                }
                else if (dominoEnded[i + 1] && new_angle < fin_angle)
                {
                    dominoV0[i] = 0;
                    dominoMoving[i] = 0;
                    dominoEnded[i] = 1;
                    dominos[i]->rotateZX(-dominos[i]->getAngle() + fin_angle);
                    new_v = 0;
                }
                dominos[i]->setVelocity(new_v);
            }
        }

        // Collision sphere and mayatnik
        if (model2->getCollisionCenter().findDistance(model1->getCollisionCenter()) <=
                model2->getCollisionRadius() + model1->getCollisionRadius())
        {
            double v0 = model1->getVelocity();
            double m1 = model1->getMass();
            double m2 = model2->getMass();
            m2_v0 = 2 * m1 / (m1 + m2) * v0;
            model1->setVelocity((m1 - m2) / (m1 + m2) * v0);
            model2->setVelocity(m2_v0);
        }

        // Collision mayatnik and domino
        if (dominos.front()->getLeftUp().getZ() >= model2->getCollisionCenter().getZ() &&
                model2->getCollisionCenter().findDistanceToLine(dominos[0]->getLeftUp(), dominos[0]->getLeftDown()) <=
                model2->getCollisionRadius())
        {
            double v0 = model2->getVelocity();
            double m1 = model2->getMass();
            double m2 = dominos[0]->getMass();
            dominoMoving[0] = 1;
            dominoV0[0] = 2 * m1 / (m1 + m2) * v0;
            model2->setVelocity((m1 - m2) / (m1 + m2) * v0);
            dominos[0]->setVelocity(dominoV0[0]);
        }

        // Collision domino and domino
        for (int i = 8; i > -1; i--)
        {
            if (!dominoEnded[i + 1] &&
                    dominos[i]->getRightUp().findDistanceToLine(dominos[i + 1]->getLeftUp(), dominos[i + 1]->getLeftDown()) <= DIST_EPS)
            {
                double v0 = dominos[i]->getVelocity();
                double m1 = dominos[i]->getMass();
                double m2 = dominos[i + 1]->getMass();
                dominoV0[i] = (m1 - m2) / (m1 + m2) * v0;
                dominoV0[i + 1] = 2 * m1 / (m1 + m2) * v0;
                dominoMoving[i + 1] = 1;
                dominos[i]->setVelocity(dominoV0[i]);
                dominos[i + 1]->setVelocity(dominoV0[i + 1]);
            }
        }

        // Collision domino and button
        if (dominos.back()->getRightUp().getZ() >= model3->getLeftDown().getZ() &&
                dominos.back()->getRightUp().findDistanceToLine(model3->getLeftUp(), model3->getLeftDown()) <= DIST_EPS)
        {
            double v0 = dominos.back()->getVelocity();
            double m1 = dominos.back()->getMass();
            double m2 = model3->getMass();
            dominoV0.back() = (m1 - m2) / (m1 + m2) * v0;
            dominos.back()->setVelocity(dominoV0.back());
            model3->setVelocity(2 * m1 / (m1 + m2) * v0, MU_BUTTON);
        }

        QGraphicsScene *setScene = drawScene(graphicsView->rect());

        if (graphicsView->scene())
            delete graphicsView->scene();
        graphicsView->setScene(setScene);

        QTime end = QTime::currentTime().addMSecs(1);
        while (QTime::currentTime() < end)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        }
    }
    isModelling = 0;
}
