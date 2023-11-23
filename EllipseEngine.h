#ifndef ELLIPSEENGINE_H
#define ELLIPSEENGINE_H

#include "drawengine.h"
#include <QVector2D>

class EllipseEngine : public DrawEngine
{
public:
    explicit EllipseEngine(QObject *parent = nullptr);
    int a=100; //长轴
    int b=150; //短轴
    QVector2D center=QVector2D(200,200);

private:
    QVector<QPointF> samplePoints();  //采样要绘制的像素点

public slots:
    void draw(QPainter* painter);
};

#endif // ELLIPSEENGINE_H
