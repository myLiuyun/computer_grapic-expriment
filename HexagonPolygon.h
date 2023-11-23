#ifndef HEXAGONPOLYGON_H
#define HEXAGONPOLYGON_H

#include "Polygon.h"

class HexagonPolygon : public Polygon
{
public:
    explicit HexagonPolygon(QVector2D size, QVector2D position=QVector2D(0,0), float rotate=0, QObject *parent = nullptr);

private:
    void generatHexgon();  //生成一个1*1的正六边形

public slots:
    virtual void drawPolygon(QPainter* painter, LineEngine* engine) override;
    void setEdgeLength(int length);
};

#endif // HEXAGONPOLYGON_H
