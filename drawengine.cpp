#include "drawengine.h"

DrawEngine::DrawEngine(QObject *parent)
    : QObject{parent}
{
    this->pen = new QPen();
    pen->setWidth(this->width);
    pen->setColor(QColor(this->color.x(),this->color.y(),this->color.z()));
}

void DrawEngine::draw(QPainter *painter)
{
    //父类中，只设置笔刷参数
    painter->setPen(*this->pen);
}


void DrawEngine::setColor(QVector4D color)
{
    this->color = color;
    this->pen->setColor(QColor(this->color.x(),this->color.y(),this->color.z(),this->color.w()));
}

void DrawEngine::setWidth(int width)
{
    this->width=width;
    pen->setWidth(this->width);
}

QVector4D DrawEngine::getColor()
{
    return this->color;
}
