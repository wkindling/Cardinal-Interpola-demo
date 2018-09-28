#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Cardinal Spline Demo");
    layoutManage();
    setFixedSize(770,580);

}

void MainWindow::layoutManage()
{
    mainWidget=new QWidget();
    this->setCentralWidget(mainWidget);
    paintArea=new PainterWidget(0);
    paintArea->setStyleSheet(QString::fromUtf8("border:1px solid black"));

    grainBox=new QSpinBox();
    tensionBox=new QDoubleSpinBox();
    drawButton=new QPushButton();
    playButton=new QPushButton();
    clearButton=new QPushButton();
    speedSlider=new QSlider(Qt::Horizontal);

    drawButton->setText("Draw");
    clearButton->setText("Clear");
    playButton->setText("Play");
    connect(drawButton,SIGNAL(clicked()),this,SLOT(drawClicked()));
    connect(clearButton,SIGNAL(clicked()),this,SLOT(clearClicked()));
    connect(playButton,SIGNAL(clicked()),this,SLOT(playClicked()));

    tensionBox->setRange(0,1);
    tensionBox->setSingleStep(0.1);
    grainBox->setRange(0,200);
    QLabel *tensiontxt=new QLabel();
    tensiontxt->setText("Tension: ");
    QLabel *speedtxt=new QLabel();
    speedtxt->setText("Speed: ");
    QLabel *graintxt=new QLabel();
    graintxt->setText("grain: ");

    speedSlider->setMinimum(1);
    speedSlider->setMaximum(10);

    sublayout=new QGridLayout();
    mainlayout=new QVBoxLayout();

    sublayout->addWidget(tensiontxt,0,0);
    sublayout->addWidget(tensionBox,0,1);
    sublayout->addWidget(drawButton,0,2);
    sublayout->addWidget(graintxt,1,0);
    sublayout->addWidget(grainBox,1,1);
    sublayout->addWidget(clearButton,1,2);
    sublayout->addWidget(speedtxt,2,0);
    sublayout->addWidget(speedSlider,2,1);
    sublayout->addWidget(playButton,2,2);
    mainlayout->addWidget(paintArea);
    mainlayout->addLayout(sublayout);
    mainWidget->setLayout(mainlayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawClicked()
{
    if (paintArea->CSpline.n>1)
    {
        paintArea->CSpline.tension=tensionBox->value();
        paintArea->CSpline.grain=grainBox->value();
        paintArea->CSpline.createSpline();
        paintArea->flag=1;
        update();
    }
}

void MainWindow::clearClicked()
{
    paintArea->flag=2;
    update();
}

void MainWindow::playClicked()
{
    if (paintArea->CSpline.splinePnt.size()>0)
    {
        paintArea->car->show();
        paintArea->speed=speedSlider->value();
        int duration=10000/paintArea->speed;
        int sum=(paintArea->CSpline.n-1)*paintArea->CSpline.grain;

        paintArea->ani=new QPropertyAnimation(paintArea->car,"pos");
        paintArea->ani->setDuration(duration);
        double u=1.0f/20;

        paintArea->ani->setStartValue(QPoint(paintArea->CSpline.splinePnt[0].x()-20,paintArea->CSpline.splinePnt[0].y()-15));
        for (int i=0;i<20;i++)
        {
            paintArea->ani->setKeyValueAt(u*i,QPoint(paintArea->CSpline.splinePnt[u*i*sum].x()-20,paintArea->CSpline.splinePnt[u*i*sum].y()-15));
        }
        paintArea->ani->setEndValue(QPoint(paintArea->CSpline.splinePnt[sum].x()-20,paintArea->CSpline.splinePnt[sum].y()-15));
        paintArea->ani->start();
    }
}


PainterWidget::PainterWidget(QWidget* parent):QWidget(parent)
{
    setFixedSize(750,400);
    setStyleSheet(QString::fromUtf8("border:1px solid black"));
    this->setMouseTracking(true);
    this->isPressed=false;

    car=new QLabel(this);
    car->setFixedSize(40,40);
    img=new QImage;
    if (!img->load(":/car/car")) qDebug()<<"Load Picture failed!";
    QImage newimg=img->scaled(car->width(),car->height());
    car->setPixmap(QPixmap::fromImage(newimg));
    car->setStyleSheet(QString::fromUtf8("border:0px solid black"));
    car->hide();
    flag=0;
}

void PainterWidget::mousePressEvent(QMouseEvent *e)
{
    QPoint temp=QPoint(e->pos());
    CSpline.ctrPnt.push_back(temp);
    CSpline.n=(int)CSpline.ctrPnt.size();
    update();
    this->isPressed=true;
}

void PainterWidget::mouseReleaseEvent(QMouseEvent *e)
{
    this->isPressed=false;
}

void PainterWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(2);
    painter.setPen(pen);

    if (flag==0)
    {
        if (CSpline.ctrPnt.size()==1) painter.drawPoint(CSpline.ctrPnt[0]);
        else if (CSpline.ctrPnt.size()>1)
            for (int i=0;i<CSpline.ctrPnt.size()-1;i++)
                painter.drawLine(CSpline.ctrPnt[i],CSpline.ctrPnt[i+1]);
    }
    else if (flag==1)
    {
        for (int i=0;i<CSpline.ctrPnt.size()-1;i++)
            painter.drawLine(CSpline.ctrPnt[i],CSpline.ctrPnt[i+1]);
        pen.setColor(Qt::red);
        pen.setWidth(3);
        painter.setPen(pen);

        for (int i=0;i<CSpline.splinePnt.size()-1;i++)
            painter.drawLine(CSpline.splinePnt[i],CSpline.splinePnt[i+1]);
        pen.setColor(Qt::blue);
        pen.setWidth(2);
        painter.setPen(pen);
    }
    else if (flag==2)
    {
        CSpline.n=0;
        CSpline.splinePnt.clear();
        CSpline.ctrPnt.clear();
        car->hide();
        flag=0;
    }
}

