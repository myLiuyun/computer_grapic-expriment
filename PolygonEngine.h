/*
自由绘制多边形的类，多边形用一组顶点序列表示
*/
#ifndef POLYGONENGINE_H
#define POLYGONENGINE_H

#include "LineEngine.h"
#include "Polygon.h"

class PolygonEngine : public LineEngine
{
public:
    explicit PolygonEngine(Polygon* polygon ,QObject *parent = nullptr);
    Polygon* polygon=nullptr; //顶点序列

public slots:
    void draw(QPainter* painter) override;
    void setPolygon(Polygon* polygon);
};

#endif // POLYGONENGINE_H
