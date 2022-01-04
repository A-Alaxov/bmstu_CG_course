#include "mainwindow.h"
#include "ui_mainwindow.h"

//#define FLEX

#include "QDebug"
#include <QErrorMessage>
#include <QShortcut>
#include <QTimer>

#ifdef FLEX
#include <QMediaPlayer>
#endif

#include "config.hpp"

#include "illuminantplacechooser.hpp"
#include "specialgraphicsview.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    facade = new UsageFacade;

#ifdef FLEX
    QMediaPlayer *player = new QMediaPlayer();
    player->setMedia(QUrl("../BigBabyTape.mp3"));
    player->play();
#endif

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget::connect(ui->graphicsView, SIGNAL(sendMouse(size_t, size_t)), this,
        SLOT(getMouseEvent(size_t, size_t)));

    QShortcut *shortcutDown = new QShortcut(QKeySequence("down"), this);
    QObject::connect(shortcutDown, SIGNAL(activated()), this, SLOT(pictureDown()));

    QShortcut *shortcutUp = new QShortcut(QKeySequence("up"), this);
    QObject::connect(shortcutUp, SIGNAL(activated()), this, SLOT(pictureUp()));

    QShortcut *shortcutLeft = new QShortcut(QKeySequence("left"), this);
    QObject::connect(shortcutLeft, SIGNAL(activated()), this, SLOT(pictureLeft()));

    QShortcut *shortcutRight = new QShortcut(QKeySequence("right"), this);
    QObject::connect(shortcutRight, SIGNAL(activated()), this, SLOT(pictureRight()));

    QShortcut *shortcutScaleUp = new QShortcut(QKeySequence("z"), this);
    QObject::connect(shortcutScaleUp, SIGNAL(activated()), this, SLOT(pictureScaleUp()));

    QShortcut *shortcutScaleDown = new QShortcut(QKeySequence("x"), this);
    QObject::connect(
        shortcutScaleDown, SIGNAL(activated()), this, SLOT(pictureScaleDown()));

    QShortcut *shortcutRotateXRight = new QShortcut(QKeySequence("s"), this);
    QObject::connect(
        shortcutRotateXRight, SIGNAL(activated()), this, SLOT(pictureRotateXRight()));

    QShortcut *shortcutRotateXLeft = new QShortcut(QKeySequence("w"), this);
    QObject::connect(
        shortcutRotateXLeft, SIGNAL(activated()), this, SLOT(pictureRotateXLeft()));

    QShortcut *shortcutRotateYRight = new QShortcut(QKeySequence("d"), this);
    QObject::connect(
        shortcutRotateYRight, SIGNAL(activated()), this, SLOT(pictureRotateYRight()));

    QShortcut *shortcutRotateYLeft = new QShortcut(QKeySequence("a"), this);
    QObject::connect(
        shortcutRotateYLeft, SIGNAL(activated()), this, SLOT(pictureRotateYLeft()));

    QShortcut *shortcutRotateZLeft = new QShortcut(QKeySequence("q"), this);
    QObject::connect(
        shortcutRotateZLeft, SIGNAL(activated()), this, SLOT(pictureRotateZLeft()));

    QShortcut *shortcutRotateZRight = new QShortcut(QKeySequence("e"), this);
    QObject::connect(
        shortcutRotateZRight, SIGNAL(activated()), this, SLOT(pictureRotateZRight()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::getMouseEvent(size_t x_, size_t y_)
{
    qDebug() << "Приняли ивент:" << x_ << y_ << '\n';
}

void MainWindow::pictureDown()
{
    if (!facade->isSceneSet())
        return;
    qDebug() << "Крутим вниз";
    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    QGraphicsScene *setScene = facade->moveDownScene(MOVE_UNIT, ui->graphicsView->rect());

    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureUp()
{
    if (!facade->isSceneSet())
        return;
    qDebug() << "Крутим вверх";
    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    QGraphicsScene *setScene = facade->moveUpScene(MOVE_UNIT, ui->graphicsView->rect());

    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureLeft()
{
    if (!facade->isSceneSet())
        return;
    qDebug() << "Крутим влево";
    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    QGraphicsScene *setScene = facade->moveLeftScene(MOVE_UNIT, ui->graphicsView->rect());

    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRight()
{
    if (!facade->isSceneSet())
        return;
    qDebug() << "Крутим вправо";
    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    QGraphicsScene *setScene =
        facade->moveRightScene(MOVE_UNIT, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureScaleUp()
{
    if (!facade->isSceneSet())
        return;
    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene =
        facade->scaleScene(SCALE_VALUE + 1, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureScaleDown()
{
    if (!facade->isSceneSet())
        return;
    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene =
        facade->scaleScene(1 - SCALE_VALUE, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRotateXRight()
{
    if (!facade->isSceneSet())
        return;
    qDebug() << "Вертим по Х вниз";
    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    QGraphicsScene *setScene =
        facade->rotateXScene(ROTATE_UNIT, ui->graphicsView->rect());

    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRotateXLeft()
{
    if (!facade->isSceneSet())
        return;
    qDebug() << "Вертим по Х";
    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    QGraphicsScene *setScene =
        facade->rotateXScene(-ROTATE_UNIT, ui->graphicsView->rect());

    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRotateYRight()
{
    if (!facade->isSceneSet())
        return;
    qDebug() << "Вертим по Y";
    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    QGraphicsScene *setScene =
        facade->rotateYScene(ROTATE_UNIT, ui->graphicsView->rect());

    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRotateYLeft()
{
    if (!facade->isSceneSet())
        return;
    qDebug() << "Вертим по Y влево";
    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    QGraphicsScene *setScene =
        facade->rotateYScene(-ROTATE_UNIT, ui->graphicsView->rect());

    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRotateZRight()
{
    if (!facade->isSceneSet())
        return;
    qDebug() << "Вертим по z";
    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    QGraphicsScene *setScene =
        facade->rotateZScene(ROTATE_UNIT, ui->graphicsView->rect());

    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRotateZLeft()
{
    if (!facade->isSceneSet())
        return;
    qDebug() << "Вертим по z";
    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    QGraphicsScene *setScene =
        facade->rotateZScene(-ROTATE_UNIT, ui->graphicsView->rect());

    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureToCenter()
{
}

void MainWindow::on_pushButton_4_clicked()
{
    if (isModelling)
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Идёт процесс моделирования.");
        return;
    }

    facade->setCellScene(13, 6);

    facade->addPendulum();
    facade->addSphere();
    facade->addCylindah();
    facade->addStand();
    facade->addButtonStand();
    for (int i = 0; i < 10; i++)
        facade->addDomino();

    facade->setObjects();

    readyModelling = 1;
    haveIllum = 0;

    QGraphicsScene *setScene = facade->drawScene(ui->graphicsView->rect());

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    ui->graphicsView->setScene(setScene);
}

void MainWindow::on_pushButton_5_clicked()
{
}

void MainWindow::on_pushButton_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Сцена ещё не была задана.");
        return;
    }

    if (isModelling)
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Идёт процесс моделирования.");
        return;
    }

    if (haveIllum)
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Источник света уже задан.");
        return;
    }


    IlluminantPlaceChooser placeChooserWindow(nullptr);
    placeChooserWindow.setModal(true);
    placeChooserWindow.exec();

    if (!placeChooserWindow.checkSet())
        return;

    haveIllum = 1;
    facade->addIlluminant(
        placeChooserWindow.getXAngle(), placeChooserWindow.getYAngle());

    QGraphicsScene *setScene = facade->drawScene(ui->graphicsView->rect());

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    ui->graphicsView->setScene(setScene);
}

void MainWindow::on_pushButton_7_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Сцена ещё не была задана.");
        return;
    }

    if (isModelling)
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Идёт процесс моделирования.");
        return;
    }

    haveIllum = 0;

    facade->getScene()->deleteIlluminant(0);
    QGraphicsScene *setScene = facade->drawScene(ui->graphicsView->rect());

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    ui->graphicsView->setScene(setScene);
}

void MainWindow::on_pushButton_8_clicked()
{
}

void MainWindow::on_pushButton_6_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Сцена ещё не была задана.");
        return;
    }

    if (isModelling)
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Идёт процесс моделирования.");
        return;
    }

    if (!readyModelling)
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Создайте новую сцену.");
        return;
    }

    isModelling = 1;
    readyModelling = 0;
    facade->modelling(ui->graphicsView);
    isModelling = 0;
}

void MainWindow::on_pushButton_9_clicked()
{
}

std::string fname = "sphere.txt";
size_t th_c = TH_COUNT;

#define CNT 1000

void MainWindow::on_pushButton_2_clicked()
{
    on_pushButton_4_clicked();
    facade->addIlluminant(10, 20);
    for (int j = 10; j < 520; j += 125)
    {
        fname = "pyramid_" + std::to_string(j) + ".txt";
        th_c = 0;
        nanoseconds cur = cur.zero();
        for  (int k = 0; k < CNT; k++)
            facade->drawScene(ui->graphicsView->rect(), &cur);
        qDebug() << j * 4 + 620 << ',' << cur.count() / 10000 / CNT << ',';

        for (int i = 1; i < 32; i *= 2)
        {
            th_c = i;
            nanoseconds cur = cur.zero();
            for  (int k = 0; k < CNT; k++)
                facade->drawScene(ui->graphicsView->rect(), &cur);
            qDebug() << cur.count() / 10000 / CNT << ',';
        }
    }
}
