#include "HexagonPolygon.h"
#include <QtMath>

HexagonPolygon::HexagonPolygon(QVector2D size, QVector2D position, float rotate, QObject *parent)
    : Polygon{parent}
{
    setPosition(position);
    setScale(size);
    setAngle(rotate);
//    this->position=position;
//    this->scale=size;
//    this->angle=rotate;
    generatHexgon();
}

void HexagonPolygon::generatHexgon()
{
    //在(-1,1)内建立一个Hexgon模板
    double rad = M_PI / 180;
    for(int i=0;i<6;i++){
        qDebug()<<QVector2D(cos(i*60*rad),sin(i*60*rad));
        addVertex(QVector2D(cos(i*60*rad),sin(i*60*rad)));
    }
}

void HexagonPolygon::drawPolygon(QPainter *painter, LineEngine *engine)
{
    Polygon::drawPolygon(painter,engine);
}

void HexagonPolygon::setEdgeLength(int length)
{
    //边长到Size的映射
    setScale(length);
}
