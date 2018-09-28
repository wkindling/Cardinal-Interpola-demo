#ifndef SPLINE_H
#define SPLINE_H

#include <vector>
#include <QPoint>

class CardinalSpline
{
public:
    CardinalSpline(){}
    ~CardinalSpline(){}

    std::vector<QPoint> splinePnt;
    std::vector<QPoint> ctrPnt;
    int n;
    double M[4][4];
    int grain;
    double tension;

    void createSpline();
    double Matrix(double p1,double p2,double p3,double p4,double u);
    void getCardinalMatrix();
};

#endif // SPLINE_H
