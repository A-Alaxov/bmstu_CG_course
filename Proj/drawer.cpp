#include "drawer.hpp"
#include <thread>
#include <mutex>


void Drawer::interpolateRowIntoShadowMap(
    std::vector<std::vector<double>> *map, int xA, int xB, double zA, double zB, int curY)
{
    for (int curX = xA; curX <= xB; curX++)
    {
        double curZ = zA + (zB - zA) * (curX - xA) / (xB - xA);
        if (curX >= (int) map->size() || curX < 0 || curY >= (int) map->at(0).size() || curY < 0)
            continue;

        if (curZ > map->at(curX).at(curY))
            map->at(curX).at(curY) = curZ;
    }
}

std::mutex mtx;

void Drawer::shadowMapForFacet(size_t start, size_t finish, std::vector<Facet> &facets, std::vector<Dot> &vertices,
    size_t bufWidth, size_t bufHeight, Illuminant *illum, PolModel *md)
{
    std::array<Dot, 3> dotsArr;
    for (size_t curFaceNum = start; curFaceNum < finish; curFaceNum++)
    {
        //Гуро
        Eigen::MatrixXf coordinatesVec(3, 4);

        std::vector<size_t> curDots = facets.at(curFaceNum).getUsedVertices();
        dotsArr[0] = vertices.at(curDots.at(0));
        dotsArr[1] = vertices.at(curDots.at(1));
        dotsArr[2] = vertices.at(curDots.at(2));

        coordinatesVec << dotsArr[0].getX(), dotsArr[0].getY(),
            dotsArr[0].getZ(), 1, dotsArr[1].getX(),
            dotsArr[1].getY(), dotsArr[1].getZ(), 1,
            dotsArr[2].getX(), dotsArr[2].getY(),
            dotsArr[2].getZ(), 1;
        coordinatesVec *= dotTransMat;
        dotsArr[0] =
            Dot(coordinatesVec(0, 0), coordinatesVec(0, 1), coordinatesVec(0, 2));
        dotsArr[1] =
            Dot(coordinatesVec(1, 0), coordinatesVec(1, 1), coordinatesVec(1, 2));
        dotsArr[2] =
            Dot(coordinatesVec(2, 0), coordinatesVec(2, 1), coordinatesVec(2, 2));

        //facets.at(curFaceNum).setIntens(find_surface_intensity(dotsArr, illum));
        md->changeIntense(curFaceNum, find_surface_intensity(dotsArr, illum));

        // Начало
        dotsArr[0] = vertices.at(curDots.at(0));
        dotsArr[1] = vertices.at(curDots.at(1));
        dotsArr[2] = vertices.at(curDots.at(2));

        coordinatesVec << dotsArr[0].getX(), dotsArr[0].getY(),
            dotsArr[0].getZ(), 1, dotsArr[1].getX(),
            dotsArr[1].getY(), dotsArr[1].getZ(), 1,
            dotsArr[2].getX(), dotsArr[2].getY(),
            dotsArr[2].getZ(), 1;

        coordinatesVec *= dotShadowTransMat;
        dotsArr[0] =
            Dot(coordinatesVec(0, 0), coordinatesVec(0, 1), coordinatesVec(0, 2));
        dotsArr[1] =
            Dot(coordinatesVec(1, 0), coordinatesVec(1, 1), coordinatesVec(1, 2));
        dotsArr[2] =
            Dot(coordinatesVec(2, 0), coordinatesVec(2, 1), coordinatesVec(2, 2));

        if (dotsArr[0].getY() > dotsArr[1].getY())
            std::swap(dotsArr[0], dotsArr[1]);
        if (dotsArr[0].getY() > dotsArr[2].getY())
            std::swap(dotsArr[0], dotsArr[2]);
        if (dotsArr[1].getY() > dotsArr[2].getY())
            std::swap(dotsArr[1], dotsArr[2]);

        int x1 = round(dotsArr[0].getX());
        int x2 = round(dotsArr[1].getX());
        int x3 = round(dotsArr[2].getX());

        double z1 = dotsArr[0].getZ();
        double z2 = dotsArr[1].getZ();
        double z3 = dotsArr[2].getZ();

        int y1 = round(dotsArr[0].getY());
        int y2 = round(dotsArr[1].getY());
        int y3 = round(dotsArr[2].getY());

        for (int curY = (y1 < 0) ? 0 : y1;
             curY < ((y2 >= (int) bufHeight) ? (int) bufHeight - 1 : y2); curY++)
        {
            double aInc = 0;
            if (y1 != y2)
                aInc = (double) (curY - y1) / (y2 - y1);

            double bInc = 0;
            if (y1 != y3)
                bInc = (double) (curY - y1) / (y3 - y1);

            int xA = round(x1 + (x2 - x1) * aInc);
            int xB = round(x1 + (x3 - x1) * bInc);
            double zA = z1 + (z2 - z1) * aInc;
            double zB = z1 + (z3 - z1) * bInc;

            if (xA > xB)
            {
                std::swap(xA, xB);
                std::swap(zA, zB);
            }

            if (xA < 0)
                xA = 0;
            if (xB >= (int) bufWidth)
                xB = (int) bufWidth - 1;

            //mtx.lock();
            interpolateRowIntoShadowMap(shadowMap, xA, xB, zA, zB, curY);
            //mtx.unlock();
        }

        for (int curY = (y2 < 0) ? 0 : y2;
             curY <= ((y3 >= (int) bufHeight) ? (int) bufHeight - 1 : y3); curY++)
        {
            double aInc = 0;
            if (y2 != y3)
                aInc = (double) (curY - y2) / (y3 - y2);

            double bInc = 0;
            if (y1 != y3)
                bInc = (double) (curY - y1) / (y3 - y1);

            int xA = round(x2 + (x3 - x2) * aInc);
            int xB = round(x1 + (x3 - x1) * bInc);
            double zA = z2 + (z3 - z2) * aInc;
            double zB = z1 + (z3 - z1) * bInc;

            if (xA > xB)
            {
                std::swap(xA, xB);
                std::swap(zA, zB);
            }

            if (xA < 0)
                xA = 0;
            if (xB >= (int) bufWidth)
                xB = (int) bufWidth - 1;

            //mtx.lock();
            interpolateRowIntoShadowMap(shadowMap, xA, xB, zA, zB, curY);
            //mtx.unlock();
        }
    }
}

void Drawer::shadowMapForModel(std::vector<Facet> &facets, std::vector<Dot> &vertices,
    Eigen::Matrix4f &transMat, Illuminant *illum, size_t bufWidth, size_t bufHeight, nanoseconds *cur, PolModel *md)
{
    std::array<Dot, 3> dotsArr;
    Eigen::Matrix4f toCenter;
    toCenter << 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                -X_CENTER, -Y_CENTER, -PLATE_Z - 5, 1;
    Eigen::Matrix4f backToStart;
    backToStart << 1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   X_CENTER, Y_CENTER, PLATE_Z + 5, 1;
    shadowMap = &illum->getShadowMap();
    Eigen::Matrix4f illumMat = illum->getTransMat();

    dotShadowTransMat = toCenter * transMat * illumMat * backToStart;

    //nanoseconds start = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());
    if (!th_c)
    {
        shadowMapForFacet(0, facets.size(), facets, vertices, bufWidth, bufHeight, illum, md);
//        nanoseconds end = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());

//        if (cur)
//            *cur += (end - start);
        return;
    }

    size_t edges_for_th = facets.size() / th_c;
    std::vector<std::thread> th;

    for (size_t i = 0; i < th_c - 1; i++)
    {
        size_t start = i * edges_for_th, finish = (i + 1) * edges_for_th;
        th.push_back(std::thread([start, finish, &facets, &vertices,
                             bufWidth, bufHeight, illum, &md, this]()
                       {
                           shadowMapForFacet(start, finish, facets, vertices,
                                        bufWidth, bufHeight, illum, md);
                       }));
    }
    th.push_back(std::thread([edges_for_th, &facets, &vertices,
                             bufWidth, bufHeight, illum, &md, this]()
                   {
                       shadowMapForFacet((th_c - 1) * edges_for_th, facets.size(), facets, vertices,
                                    bufWidth, bufHeight, illum, md);
                   }));

    for (size_t i = 0; i < th_c; i++)
    {
        th[i].join();
    }
//    nanoseconds end = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());

//    if (cur)
//        *cur += (end - start);
    th.clear();
}

void Drawer::zBufForFacet(size_t start, size_t finish, std::vector<Facet> &facets, std::vector<Dot> &vertices,
                          CellScene *scene, size_t bufWidth, size_t bufHeight)
{
    std::array<Dot, 3> dotsArr;
    for (size_t curFaceNum = start; curFaceNum < finish; curFaceNum++)
    {
        Eigen::MatrixXf coordinatesVec(3, 4);

        std::vector<size_t> curDots = facets.at(curFaceNum).getUsedVertices();
        dotsArr[0] = vertices.at(curDots.at(0));
        dotsArr[1] = vertices.at(curDots.at(1));
        dotsArr[2] = vertices.at(curDots.at(2));

        std::vector<double> intensArr;
        for (auto dot : dotsArr)
        {
            if (facets.at(curFaceNum).checkGuro())
            {
                double intSum = 0, cnt = 0;
                for (auto i : dot.getFacets())
                {
                    intSum += facets.at(i).getIntens();
                    cnt++;
                }
                intensArr.push_back(intSum / cnt);
            }
            else
            {
                intensArr.push_back(facets.at(curFaceNum).getIntens());
            }
        }

        coordinatesVec << dotsArr[0].getX(), dotsArr[0].getY(),
            dotsArr[0].getZ(), 1, dotsArr[1].getX(),
            dotsArr[1].getY(), dotsArr[1].getZ(), 1,
            dotsArr[2].getX(), dotsArr[2].getY(),
            dotsArr[2].getZ(), 1;
        coordinatesVec *= dotTransMat;
        dotsArr[0] =
            Dot(coordinatesVec(0, 0), coordinatesVec(0, 1), coordinatesVec(0, 2));
        dotsArr[1] =
            Dot(coordinatesVec(1, 0), coordinatesVec(1, 1), coordinatesVec(1, 2));
        dotsArr[2] =
            Dot(coordinatesVec(2, 0), coordinatesVec(2, 1), coordinatesVec(2, 2));

        if (dotsArr[0].getY() > dotsArr[1].getY()){
            std::swap(dotsArr[0], dotsArr[1]);
            std::swap(intensArr[0], intensArr[1]);
        }
        if (dotsArr[0].getY() > dotsArr[2].getY()){
            std::swap(dotsArr[0], dotsArr[2]);
            std::swap(intensArr[0], intensArr[2]);
        }
        if (dotsArr[1].getY() > dotsArr[2].getY()){
            std::swap(dotsArr[1], dotsArr[2]);
            std::swap(intensArr[1], intensArr[2]);
        }

        int x1 = round(dotsArr[0].getX());
        int x2 = round(dotsArr[1].getX());
        int x3 = round(dotsArr[2].getX());

        double i1 = intensArr[0];
        double i2 = intensArr[1];
        double i3 = intensArr[2];

        double z1 = dotsArr[0].getZ();
        double z2 = dotsArr[1].getZ();
        double z3 = dotsArr[2].getZ();

        int y1 = round(dotsArr[0].getY());
        int y2 = round(dotsArr[1].getY());
        int y3 = round(dotsArr[2].getY());

        for (int curY = (y1 < 0) ? 0 : y1;
             curY < ((y2 >= (int) bufHeight) ? (int) bufHeight - 1 : y2); curY++)
        {
            double aInc = 0;
            if (y1 != y2)
                aInc = (double) (curY - y1) / (y2 - y1);

            double bInc = 0;
            if (y1 != y3)
                bInc = (double) (curY - y1) / (y3 - y1);

            int xA = round(x1 + (x2 - x1) * aInc);
            int xB = round(x1 + (x3 - x1) * bInc);
            double zA = z1 + (z2 - z1) * aInc;
            double zB = z1 + (z3 - z1) * bInc;
            double iA = i1 + (i2 - i1) * aInc;
            double iB = i1 + (i3 - i1) * bInc;

            if (xA > xB)
            {
                std::swap(xA, xB);
                std::swap(zA, zB);
                std::swap(iA, iB);
            }

            if (xA < 0)
                xA = 0;
            if (xB >= (int) bufWidth)
                xB = (int) bufWidth - 1;

            for (int curX = xA; curX <= xB; curX++)
            {
                double curZ = zA + (zB - zA) * (curX - xA) / (xB - xA);
                double curI = iA + (iB - iA) * (curX - xA) / (xB - xA);
                //mtx.lock();
                if (curZ >= depthBuffer.at(curX).at(curY))
                {
                    short visible = 0;
                    Eigen::MatrixXf newCoordinates(1, 4);
                    for (size_t i = 0; i < scene->getIllumNum() && !visible; i++)
                    {
                        newCoordinates << curX, curY, curZ, 1;

                        newCoordinates *= illumDotMatrices.at(i);
                        std::vector<std::vector<double>> *shadowMap =
                            &scene->getIlluminant(i).getShadowMap();
                        int x = round(newCoordinates(0, 0));
                        int y = round(newCoordinates(0, 1));
//                        if (curY > 849)
//                            printf("%d %d", y, curY);
                        if (x < (int) shadowMap->size() && x >= 0 &&
                            y < (int) shadowMap->at(0).size() && y >= 0 &&
                            std::fabs(shadowMap->at(x).at(y) - newCoordinates(0, 2)) < 2)
                            visible = 1;
                    }
                    depthBuffer.at(curX).at(curY) = curZ;
                    if (scene->getIllumNum())
                    {
                        frameBuffer.at(curX).at(curY) = facets.at(curFaceNum).getColor() + visible;
                        intensBuffer.at(curX).at(curY) = curI;
                        //intensBuffer.at(curX).at(curY) = intens;
                    }
                    else
                        frameBuffer.at(curX).at(curY) = facets.at(curFaceNum).getColor() + 1;
                }
                //mtx.unlock();
            }
        }

        for (int curY = (y2 < 0) ? 0 : y2;
             curY <= ((y3 >= (int) bufHeight) ? (int) bufHeight - 1 : y3); curY++)
        {
            double aInc = 0;
            if (y2 != y3)
                aInc = (double) (curY - y2) / (y3 - y2);

            double bInc = 0;
            if (y1 != y3)
                bInc = (double) (curY - y1) / (y3 - y1);

            int xA = round(x2 + (x3 - x2) * aInc);
            int xB = round(x1 + (x3 - x1) * bInc);
            double zA = z2 + (z3 - z2) * aInc;
            double zB = z1 + (z3 - z1) * bInc;
            double iA = i2 + (i3 - i2) * aInc;
            double iB = i1 + (i3 - i1) * bInc;

            if (xA > xB)
            {
                std::swap(xA, xB);
                std::swap(zA, zB);
                std::swap(iA, iB);
            }

            if (xA < 0)
                xA = 0;
            if (xB >= (int) bufWidth)
                xB = (int) bufWidth - 1;

            for (int curX = xA; curX <= xB; curX++)
            {
                double curZ = zA + (zB - zA) * (curX - xA) / (xB - xA);
                double curI = iA + (iB - iA) * (curX - xA) / (xB - xA);
                //mtx.lock();
                if (curZ >= depthBuffer.at(curX).at(curY))
                {
                    short visible = 0;
                    Eigen::MatrixXf newCoordinates(1, 4);
                    for (size_t i = 0; i < scene->getIllumNum() && !visible; i++)
                    {
                        newCoordinates << curX, curY, curZ, 1;

                        newCoordinates *= illumDotMatrices.at(i);
                        std::vector<std::vector<double>> *shadowMap =
                            &scene->getIlluminant(i).getShadowMap();
                        int x = round(newCoordinates(0, 0));
                        int y = round(newCoordinates(0, 1));
//                        if (curY > 849)
//                            printf("%d %d", y, curY);
                        if (x < (int) shadowMap->size() && x >= 0 &&
                            y < (int) shadowMap->at(0).size() && y >= 0 &&
                            std::fabs(shadowMap->at(x).at(y) - newCoordinates(0, 2)) < 2)
                            visible = 1;
                    }
                    depthBuffer.at(curX).at(curY) = curZ;
                    if (scene->getIllumNum())
                    {
                        frameBuffer.at(curX).at(curY) = facets.at(curFaceNum).getColor() + visible;
                        intensBuffer.at(curX).at(curY) = curI;
                        //intensBuffer.at(curX).at(curY) = intens;
                    }
                    else
                        frameBuffer.at(curX).at(curY) = facets.at(curFaceNum).getColor() + 1;
                }
                //mtx.unlock();
            }
        }
    }
}

double Drawer::find_surface_intensity(std::array<Dot, 3> cur_points, Illuminant *illum)
{
    Dot vec_1 = cur_points[1] - cur_points[0];
    Dot vec_2 = cur_points[2] - cur_points[1];

    Dot vec_mlt = vec_1 ^ vec_2;

    double x = sin((double) illum->getYAngle() * M_PI / 180.0),
            y = sin((double) illum->getXAngle() * M_PI / 180.0);
    Dot source_vec = Dot(x, y, sqrt(1 - x * x - y * y));


    double source_angle = vec_mlt.vector_angle(source_vec);
    double light_intensity = abs(cos(source_angle)) - 1;

    return light_intensity;
}

void Drawer::zBufForModel(std::vector<Facet> &facets, std::vector<Dot> &vertices,
    CellScene *scene, size_t bufWidth,
    size_t bufHeight, nanoseconds *cur)
{
    std::array<Dot, 3> dotsArr;
    Eigen::Matrix4f toCenter;
    toCenter << 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                -X_CENTER, -Y_CENTER, -PLATE_Z - 5, 1;
    Eigen::Matrix4f backToStart;
    backToStart << 1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   X_CENTER, Y_CENTER, PLATE_Z + 5, 1;

    for (size_t i = 0; i < scene->getIllumNum(); i++)
        illumDotMatrices.push_back(
            toCenter * scene->getIlluminant(i).getTransMat() * backToStart);

    //nanoseconds start = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());
    if (!th_c)
    {
        zBufForFacet(0, facets.size(), facets, vertices, scene, bufWidth, bufHeight);
//        nanoseconds end = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());

//        if (cur)
//            *cur += (end - start);
        return;
    }

    size_t facets_for_th = facets.size() / th_c;
    std::vector<std::thread> th;

    for (size_t i = 0; i < th_c - 1; i++)
    {
        size_t start = i * facets_for_th, finish = (i + 1) * facets_for_th;
        th.push_back(std::thread([start, finish, &facets, &vertices,
                             scene, bufWidth, bufHeight, this]()
                       {
                           zBufForFacet(start, finish, facets, vertices,
                                        scene, bufWidth, bufHeight);
                       }));
    }
    th.push_back(std::thread([facets_for_th, &facets, &vertices,
                             scene, bufWidth, bufHeight, this]()
                   {
                       zBufForFacet((th_c - 1) * facets_for_th, facets.size(), facets, vertices,
                                    scene, bufWidth, bufHeight);
                   }));

    for (size_t i = 0; i < th_c; i++)
    {
        th[i].join();
    }
//    nanoseconds end = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());

//    if (cur)
//        *cur += (end - start);
    th.clear();
}

void Drawer::zBufferAlg(CellScene *scene, size_t bufHeight, size_t bufWidth, nanoseconds *cur)
{
    depthBuffer.erase(depthBuffer.begin(), depthBuffer.end());
    frameBuffer.erase(frameBuffer.begin(), frameBuffer.end());
    intensBuffer.erase(intensBuffer.begin(), intensBuffer.end());
    illumDotMatrices.erase(illumDotMatrices.begin(), illumDotMatrices.end());

    for (size_t i = 0; i < bufWidth; i++)
    {
        depthBuffer.push_back(std::vector<double>(bufHeight, 0));
        frameBuffer.push_back(std::vector<size_t>(bufHeight, 0));
        intensBuffer.push_back(std::vector<double>(bufHeight, 0));
    }

    Eigen::Matrix4f toCenter;
    toCenter << 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                -X_CENTER, -Y_CENTER, -PLATE_Z - 5, 1;
    Eigen::Matrix4f backToStart;
    backToStart << 1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   X_CENTER, Y_CENTER, PLATE_Z + 5, 1;

    dotTransMat = toCenter * scene->getTransMatrix() * backToStart;

    PolModel model;
    std::vector<Facet> facets;
    std::vector<Dot> vertices;
    for (size_t i = 0; i < scene->getModelsNum(); i++)
    {
        PolModel *model = scene->getCoolModel(i);
        facets = model->getFacets();
        vertices = model->getVertices();
        for (size_t j = 0; j < scene->getIllumNum(); j++)
            shadowMapForModel(facets, vertices, scene->getTransMatrix(),
                &scene->getIlluminant(j), ILLUM_VIS_X, ILLUM_VIS_Y, cur, model);
    }

    PolModel *cmodel = scene->getCoolPlateModel();
    facets = cmodel->getFacets();
    vertices = cmodel->getVertices();
    for (size_t j = 0; j < scene->getIllumNum(); j++)
        shadowMapForModel(facets, vertices, scene->getTransMatrix(),
            &scene->getIlluminant(j), ILLUM_VIS_X, ILLUM_VIS_Y, cur, cmodel);

    for (size_t i = 0; i < scene->getModelsNum(); i++)
    {
        model = scene->getModel(i);
        facets = model.getFacets();
        vertices = model.getVertices();
        zBufForModel(
            facets, vertices, scene, bufWidth, bufHeight, cur);
    }
    model = scene->getPlateModel();
    facets = model.getFacets();
    vertices = model.getVertices();
    zBufForModel(
        facets, vertices, scene, bufWidth, bufHeight);

    for (size_t i = 0; i < scene->getIllumNum(); i++)
        scene->getIlluminant(i).clearShadowMap();
}

QGraphicsScene *Drawer::drawScene(CellScene *scene, QRectF rect, nanoseconds *cur)
{
    size_t width = scene->getWidth() * SCALE_FACTOR;
    size_t height = scene->getHeight() * SCALE_FACTOR;

    scene->buildPlateModel(Dot(PLATE_START), Dot(width, height, PLATE_Z));

    zBufferAlg(scene, rect.size().height(), rect.size().width(), cur);
    //qDebug() << "Time for zBuf" << (end - start).count();

    QGraphicsScene *outScene = new QGraphicsScene;
    outScene->setSceneRect(rect);

    QPen blackPen(Qt::black);

    //nanoseconds start2 = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());

    QImage *image =
        new QImage(rect.size().width(), rect.size().height(), QImage::Format_RGB32);
    image->fill(Qt::white);
    uint plateCol = qRgb(BROWN_COLOUR);
    uint darkPlateCol = qRgb(DARK_BROWN_COLOUR);
    uint blackCol = qRgb(BLACK_COLOUR);
    uint whiteCol = qRgb(WHITE_COLOUR);
    uint softBlackCol = qRgb(SOFT_BLACK_COLOUR);
    uint darkBlackCol = qRgb(DARK_BLACK_COLOUR);
    uint darkWhiteCol = qRgb(DARK_WHITE_COLOUR);
    uint modelsCol = qRgb(ORANGE_COLOUR);
    uint darkModelsCol = qRgb(DARK_ORANGE_COLOUR);
    uint grayCol = qRgb(GRAY_COLOUR);
    uint darkGrayCol = qRgb(DARK_GRAY_COLOUR);
    uint redCol = qRgb(RED_COLOUR);
    uint darkRedCol = qRgb(DARK_RED_COLOUR);
    uint silverCol = qRgb(SILVER_COLOUR);
    uint darkSilverCol = qRgb(DARK_SILVER_COLOUR);
    uint yellowCol = qRgb(YELLOW_COLOUR);
    uint darkYellowCol = qRgb(DARK_YELLOW_COLOUR);
    uint blueCol = qRgb(BLUE_COLOUR);
    uint darkBlueCol = qRgb(DARK_BLUE_COLOUR);

    for (size_t i = 0; i < rect.size().width() - 1; i++)
        for (size_t j = 0; j < rect.size().height() - 1; j++)
        {
            if (frameBuffer.at(i).at(j) == 1)
            {
                image->setPixel(i, j, darkPlateCol);
            }
            else if (frameBuffer.at(i).at(j) == 2)
            {
                //image->setPixel(i, j, plateCol);
                Dot newCol = Dot(BROWN_COLOUR).toRGB(intensBuffer.at(i).at(j) * SHADOW_INTENSITY);
                image->setPixel(i, j, qRgb(newCol.getX(), newCol.getY(), newCol.getZ()));
            }
            else if (frameBuffer.at(i).at(j) == 3)
            {
                image->setPixel(i, j, darkModelsCol);
            }
            else if (frameBuffer.at(i).at(j) == 4)
            {
                //image->setPixel(i, j, modelsCol);
                Dot newCol = Dot(ORANGE_COLOUR).toRGB(intensBuffer.at(i).at(j) * SHADOW_INTENSITY);
                image->setPixel(i, j, qRgb(newCol.getX(), newCol.getY(), newCol.getZ()));
            }
            else if (frameBuffer.at(i).at(j) == 5)
            {
                image->setPixel(i, j, blackCol);
            }
            else if (frameBuffer.at(i).at(j) == 6)
            {
                image->setPixel(i, j, darkBlackCol);
            }
            else if (frameBuffer.at(i).at(j) == 7)
            {
                //image->setPixel(i, j, softBlackCol);
                Dot newCol = Dot(SOFT_BLACK_COLOUR).toRGB(intensBuffer.at(i).at(j) * SHADOW_INTENSITY);
                image->setPixel(i, j, qRgb(newCol.getX(), newCol.getY(), newCol.getZ()));
            }
            else if (frameBuffer.at(i).at(j) == 8)
            {
                image->setPixel(i, j, darkWhiteCol);
            }
            else if (frameBuffer.at(i).at(j) == 9)
            {
                //image->setPixel(i, j, whiteCol);
                Dot newCol = Dot(WHITE_COLOUR).toRGB(intensBuffer.at(i).at(j) * SHADOW_INTENSITY);
                image->setPixel(i, j, qRgb(newCol.getX(), newCol.getY(), newCol.getZ()));
            }
            else if (frameBuffer.at(i).at(j) == 10)
            {
                image->setPixel(i, j, darkGrayCol);
            }
            else if (frameBuffer.at(i).at(j) == 11)
            {
                //image->setPixel(i, j, grayCol);
                Dot newCol = Dot(GRAY_COLOUR).toRGB(intensBuffer.at(i).at(j) * SHADOW_INTENSITY);
                image->setPixel(i, j, qRgb(newCol.getX(), newCol.getY(), newCol.getZ()));
            }
            else if (frameBuffer.at(i).at(j) == 12)
            {
                image->setPixel(i, j, darkRedCol);
            }
            else if (frameBuffer.at(i).at(j) == 13)
            {
                //image->setPixel(i, j, redCol);
                Dot newCol = Dot(RED_COLOUR).toRGB(intensBuffer.at(i).at(j) * SHADOW_INTENSITY);
                image->setPixel(i, j, qRgb(newCol.getX(), newCol.getY(), newCol.getZ()));
            }
            else if (frameBuffer.at(i).at(j) == 14)
            {
                image->setPixel(i, j, darkSilverCol);
            }
            else if (frameBuffer.at(i).at(j) == 15)
            {
                //image->setPixel(i, j, silverCol);
                Dot newCol = Dot(SILVER_COLOUR).toRGB(intensBuffer.at(i).at(j) * SHADOW_INTENSITY);
                image->setPixel(i, j, qRgb(newCol.getX(), newCol.getY(), newCol.getZ()));
            }
            else if (frameBuffer.at(i).at(j) == 16)
            {
                image->setPixel(i, j, darkYellowCol);
            }
            else if (frameBuffer.at(i).at(j) == 17)
            {
                //image->setPixel(i, j, yellowCol);
                Dot newCol = Dot(YELLOW_COLOUR).toRGB(intensBuffer.at(i).at(j) * SHADOW_INTENSITY);
                image->setPixel(i, j, qRgb(newCol.getX(), newCol.getY(), newCol.getZ()));
            }
            else if (frameBuffer.at(i).at(j) == 18)
            {
                image->setPixel(i, j, darkBlueCol);
            }
            else if (frameBuffer.at(i).at(j) == 19)
            {
                //image->setPixel(i, j, blueCol);
                Dot newCol = Dot(BLUE_COLOUR).toRGB(intensBuffer.at(i).at(j) * SHADOW_INTENSITY);
                image->setPixel(i, j, qRgb(newCol.getX(), newCol.getY(), newCol.getZ()));
            }
        }

    //nanoseconds end2 = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());
    //qDebug() << "Time for drawing" << (end2 - start2).count();

    // qDebug() << "Total time" << (end2 - start2).count() + (end - start).count();
    outScene->addPixmap(QPixmap::fromImage(*image));
    delete image;
    for (size_t i = 0; i < scene->getIllumNum(); i++)
        scene->getIlluminant(i).clearShadowMap();
    return outScene;
}
