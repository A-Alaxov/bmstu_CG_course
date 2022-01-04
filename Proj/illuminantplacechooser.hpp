#ifndef ILLUMINANTPLACECHOOSER_HPP
#define ILLUMINANTPLACECHOOSER_HPP

#include <QDialog>

namespace Ui {
class IlluminantPlaceChooser;
}

class IlluminantPlaceChooser : public QDialog
{
    Q_OBJECT

public:
    explicit IlluminantPlaceChooser(QWidget *parent = nullptr);
    ~IlluminantPlaceChooser();

    void setXAngle(int degrees);
    int getXAngle();

    void setYAngle(int degrees);
    int getYAngle();

    bool checkSet() { return isSet; }

private slots:
    void on_buttonBox_4_accepted();

    void on_buttonBox_4_rejected();

private:
    Ui::IlluminantPlaceChooser *ui;

    int xAngle = 0;
    int yAngle = 0;
    bool isSet = 0;
};

#endif // ILLUMINANTPLACECHOOSER_HPP
