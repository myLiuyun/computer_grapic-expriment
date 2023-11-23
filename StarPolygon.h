#ifndef STARPOLYGON_H
#define STARPOLYGON_H

#include "Polygon.h"

class StarPolygon : public Polygon
{
public:
    explicit StarPolygon(int size,QVector2D position=QVector2D(0,0), float rotate=0, QObject *parent = nullptr);

private:
    void pentagram(int R, int x, int y, int yDegree = 0);

public slots:
    virtual void drawPolygon(QPainter* painter, LineEngine* engine) override;

};

#endif // STARPOLYGON_H
