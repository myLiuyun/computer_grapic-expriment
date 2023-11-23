#ifndef BOUNDBOXINTERFACE_H
#define BOUNDBOXINTERFACE_H

#include <QVector2D>
#include <QVector>
#include "LineEngine.h"
#include <QPainter>
/*
包围盒的接口，实现2维包围盒的操作接口
*/

struct Boundbox{
public:
    QVector2D leftUp=QVector2D(10000,10000);
    QVector2D leftDown=QVector2D(10000,-10000);
    QVector2D rightUp=QVector2D(-10000,10000);
    QVector2D rightDown=QVector2D(-10000,-10000);
public slots:
    bool inBox(QVector2D pos);
    float getXMin();
    float getXMax();
    float getYMin();
    float getYMax();
    QVector2D getCenter();
};

class BoundboxInterface
{
public:
    BoundboxInterface();
    BoundboxInterface(const QVector<QVector2D>& vertices);
    BoundboxInterface(const QVector2D& start,const QVector2D& end);

protected:
    QVector2D leftUp=QVector2D(10000,10000);
    QVector2D leftDown=QVector2D(10000,-10000);
    QVector2D rightUp=QVector2D(-10000,10000);
    QVector2D rightDown=QVector2D(-10000,-10000);

public slots:
    float getXMin();
    float getXMax();
    float getYMin();
    float getYMax();
    QVector2D getLeftUp();
    QVector2D getLeftDown();
    QVector2D getRightUp();
    QVector2D getRightDown();
    QVector2D getCenter();
    QVector<QVector2D> getBoxVertices();  //逆时针的顶点顺序
    void updateBoundBox(const QVector<QVector2D>& vertices);  //更新包围盒
    void updateBoundBox(const QVector2D& vertex);
    virtual void drawBoundBox(QPainter* painter, LineEngine* engine)=0;
    virtual QVector<QVector2D> getBoxVerticesTransformed()=0;   //获取变换后的顶点坐标
    virtual Boundbox generateBoundBox()=0;  //根据顶点坐标，生成一个Boundbox

};

#endif // BOUNDBOXINTERFACE_H
