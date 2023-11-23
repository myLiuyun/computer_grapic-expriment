#include "SquarePolygon.h"


SquarePolygon::SquarePolygon(QVector2D position, QVector2D scale,float rotate, QObject *parent)
    : Polygon(parent)
{
//    this->position=position;
//    this->scale=scale;
//    this->angle=rotate;
    setPosition(position);
    setScale(scale);
    setAngle(rotate);
    generateSquare();
}

void SquarePolygon::generateSquare()
{
    addVertex(QVector2D(-1,-1));
    addVertex(QVector2D(-1,1));
    addVertex(QVector2D(1,1));
    addVertex(QVector2D(1,-1));
}

void SquarePolygon::drawPolygon(QPainter *painter, LineEngine *engine)
{
    //qDebug()<<getVertices();
    Polygon::drawPolygon(painter,engine);
}

void SquarePolygon::setEdgeLength(int length)
{
    setScale(length/2);
}
