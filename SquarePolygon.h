#ifndef SQUAREPOLYGON_H
#define SQUAREPOLYGON_H

#include "Polygon.h"

class SquarePolygon : public Polygon
{
public:
    explicit SquarePolygon(QVector2D position=QVector2D(0,0),QVector2D scale=QVector2D(1,1),float rotate=0 ,QObject *parent = nullptr);


private:

    void generateSquare(); //生成一个1*1的Square的顶点坐标

public slots:
    virtual void drawPolygon(QPainter* painter, LineEngine* engine) override;
    void setEdgeLength(int length);
};

#endif // SQUAREPOLYGON_H
