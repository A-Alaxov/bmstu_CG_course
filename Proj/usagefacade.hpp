#ifndef USAGEFACADE_HPP
#define USAGEFACADE_HPP

#include "specialgraphicsview.hpp"
#include "drawer.hpp"

class UsageFacade
{
public:
    UsageFacade();

    void setCellScene(size_t width_, size_t height_);
    bool isSceneSet();

    QGraphicsScene *drawScene(QRectF rect, nanoseconds *cur = nullptr);

    int addCylindah();
    int addDomino();
    int addPendulum();
    int addStand();
    int addButtonStand();
    int addSphere();

    void addIlluminant(int xAngle, int yAngle);

    void modelling(SpecialGraphicsView *graphicsView);
    void setObjects();

    QGraphicsScene *moveUpScene(double value, QRectF rect);
    QGraphicsScene *moveDownScene(double value, QRectF rect);
    QGraphicsScene *moveLeftScene(double value, QRectF rect);
    QGraphicsScene *moveRightScene(double value, QRectF rect);

    QGraphicsScene *scaleScene(double value, QRectF rect);

    QGraphicsScene *rotateXScene(double angle, QRectF rect);
    QGraphicsScene *rotateYScene(double angle, QRectF rect);
    QGraphicsScene *rotateZScene(double angle, QRectF rect);

    QGraphicsScene *toCenter(QRectF rect);

    CellScene *getScene();

private:
    CellScene *scene = nullptr;

    void addParallelepiped(std::vector<Dot> &vertices, std::vector<Facet> &facets,
        int x1, int y1, int z1, int x2, int y2, int z2, size_t color = ORANGE);
    void addQuad(std::vector<Dot> &vertices, std::vector<Facet> &facets, int x1,
    int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4, int y4,
    int z4, size_t color = ORANGE);
    void addTriangle(std::vector<Dot> &vertices, std::vector<Facet> &facets, int x1,
                     int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, size_t color = ORANGE);

    bool isModelling = 0;

    Drawer *drawer;
};

#endif // USAGEFACADE_HPP
