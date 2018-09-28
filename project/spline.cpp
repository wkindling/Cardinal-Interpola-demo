#include "spline.h"

void CardinalSpline::getCardinalMatrix()
{
    M[0][0]=-tension; M[0][1]=2-tension; M[0][2]=tension-2;   M[0][3]=tension;
    M[1][0]=2*tension;M[1][1]=tension-3; M[1][2]=3-2*tension; M[1][3]=-tension;
    M[2][0]=-tension; M[2][1]=0;   M[2][2]=tension;     M[2][3]=0;
    M[3][0]=0;  M[3][1]=1;   M[3][2]=0;     M[3][3]=0;
}

double CardinalSpline::Matrix(double p1, double p2, double p3, double p4, double u)
{
    double row1,row2,row3,row4;
    row1=p1*M[0][0]+p2*M[0][1]+p3*M[0][2]+p4*M[0][3];
    row2=p1*M[1][0]+p2*M[1][1]+p3*M[1][2]+p4*M[1][3];
    row3=p1*M[2][0]+p2*M[2][1]+p3*M[2][2]+p4*M[2][3];
    row4=p1*M[3][0]+p2*M[3][1]+p3*M[3][2]+p4*M[3][3];

    double multiple=((row1*u+row2)*u+row3)*u+row4;
    return multiple;
}

void CardinalSpline::createSpline()
{
    QPoint* knots=(QPoint*)malloc(sizeof(QPoint)*(n+2));
    double* alpha=(double*)malloc(sizeof(double)*grain);
    std::vector<QPoint> tempSpline;

    for (int i=0;i<grain;i++)
        alpha[i]=((double)i)/grain;

    getCardinalMatrix();

    for (int i=1;i<=n;i++)
        knots[i]=ctrPnt[i-1];
    knots[0]=ctrPnt[0];
    knots[n+1]=ctrPnt[n-1];

    for (int i=1;i<n;i++)
    {
        for (int j=0;j<grain;j++)
        {
            QPoint temp;
            int x=Matrix(knots[i-1].x(),knots[i].x(),knots[i+1].x(),knots[i+2].x(),alpha[j]);
            int y=Matrix(knots[i-1].y(),knots[i].y(),knots[i+1].y(),knots[i+2].y(),alpha[j]);
            temp.setX(x);temp.setY(y);
            tempSpline.push_back(temp);
        }
    }
    tempSpline.push_back(knots[n]);
    splinePnt.swap(tempSpline);
}

