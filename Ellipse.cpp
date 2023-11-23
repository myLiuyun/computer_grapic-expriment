#include "Ellipse.h"

Ellipse::Ellipse(QObject *parent)
    : QObject{parent}
{

}

Ellipse::Ellipse(QVector2D pos, int a, int b, QObject *parent)
    : QObject{parent}
{
    this->a=a;
    this->b=b;
    this->center=pos;
}

void Ellipse::setA(int a)
{
    this->a = a;
}

void Ellipse::setB(int b)
{
    this->b = b;
}

void Ellipse::setCenter(QVector2D pos)
{
    this->center=pos;
}

void Ellipse::drawPolygon(QPainter *painter, EllipseEngine *engine)
{
    engine->a=a;
    engine->b=b;
    engine->center=center;
    engine->draw(painter);
}
