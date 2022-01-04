#ifndef DRAWER_HPP
#define DRAWER_HPP

#include "objects.hpp"
#include "surface.hpp"
#include "model_file_loader.hpp"
#include "config.hpp"
#include <QGraphicsScene>

using namespace std::chrono;

extern std::string fname;
extern size_t th_c;


class Drawer
{
public:
    void zBufferAlg(CellScene *scene, size_t bufheight, size_t bufWidth, nanoseconds *cur = nullptr);
    void zBufForModel(std::vector<Facet> &facets, std::vector<Dot> &vertices,
    CellScene *scene, size_t bufWidth, size_t bufHeight, nanoseconds *cur = nullptr);
    void shadowMapForModel(std::vector<Facet> &facets, std::vector<Dot> &vertices,
    Eigen::Matrix4f &transMat, Illuminant *illum, size_t bufWidth, size_t bufHeight, nanoseconds *cur, PolModel *md);

    QGraphicsScene *drawScene(CellScene *scene, QRectF rect, nanoseconds *cur = nullptr);

private:
    double find_surface_intensity(std::array<Dot, 3> cur_points, Illuminant *illum);
    void shadowMapForFacet(size_t start, size_t finish, std::vector<Facet> &facets, std::vector<Dot> &vertices,
        size_t bufWidth, size_t bufHeight, Illuminant *illum, PolModel *md);
    void zBufForFacet(size_t start, size_t finish, std::vector<Facet> &facets, std::vector<Dot> &vertices,
                      CellScene *scene, size_t bufWidth, size_t bufHeight);
    void interpolateRowIntoShadowMap(std::vector<std::vector<double>> *map, int xA, int xB, double zA, double zB, int curY);

    Eigen::Matrix4f dotTransMat;
    Eigen::Matrix4f dotShadowTransMat;
    std::vector<Eigen::Matrix4f> illumDotMatrices;

    std::vector<std::vector<double>> *shadowMap;
    std::vector<std::vector<double>> depthBuffer;
    std::vector<std::vector<double>> intensBuffer;
    std::vector<std::vector<size_t>> frameBuffer;
};


#endif // DRAWER_HPP
