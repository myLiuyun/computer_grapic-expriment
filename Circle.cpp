#include "Circle.h"

Circle::Circle(QObject *parent)
    : QObject{parent}
{

}

Circle::Circle(int randians, QVector2D center)
{
    this->radians=randians;
    this->center=center;
}

void Circle::drawPolygon(QPainter *painter, CircleEngine *engine)
{
    engine->setCenter(this->center);
    engine->setRadians(this->radians);
    engine->draw(painter);
}

void Circle::setRadians(float rand)
{
    this->radians=rand;
}

void Circle::setCenter(QVector2D pos)
{
    this->center=pos;
}
