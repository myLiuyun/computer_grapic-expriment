/*
针对多边形区域填充的引擎类
接受一个Polygan对象
1、计算包围盒
2、从包围盒的第一行开始，逐行扫描填充
*/
#ifndef POLYGONFILLENGINE_H
#define POLYGONFILLENGINE_H

#include "drawengine.h"
#include "Polygon.h"

class PolygonFillEngine : public DrawEngine
{
public:
    explicit PolygonFillEngine(QObject *parent = nullptr);

public slots:
    void draw(QPainter* painter) override;
    void scanpolygonFill(Polygon* polygon, QPainter*painter);  //填充指定的多边形
    void fenceFill(Polygon* polygon, QPainter*painter);  //栅栏填充
};

#endif // POLYGONFILLENGINE_H
