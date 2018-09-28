#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Spline.h"
#include <QPainter>
#include <QPoint>
#include <QString>
#include <QPen>
#include <QMouseEvent>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QSlider>

namespace Ui {
class MainWindow;
}

class PainterWidget:public QWidget
{
public:
    PainterWidget(QWidget *parent);
    ~PainterWidget(){}

    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    bool isPressed;
    int flag;
    int speed;

    CardinalSpline CSpline;
    QLabel *car;
    QImage *img;
    QPropertyAnimation *ani;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QWidget *mainWidget;
    //Widget
    PainterWidget *paintArea;
    QSpinBox *grainBox;
    QDoubleSpinBox *tensionBox;
    QPushButton *drawButton;
    QPushButton *playButton;
    QPushButton *clearButton;
    QSlider *speedSlider;
    //Layout
    QVBoxLayout *mainlayout;
    QGridLayout *sublayout;


private slots:
    void drawClicked();
    void playClicked();
    void clearClicked();

private:
    Ui::MainWindow *ui;
    void layoutManage();
};

#endif // MAINWINDOW_H
