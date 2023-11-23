#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QObject>
#include <QVector2D>
#include <QPainter>
#include "EllipseEngine.h"

class Ellipse : public QObject
{
    Q_OBJECT
public:
    explicit Ellipse(QObject *parent = nullptr);
    Ellipse(QVector2D pos,int a, int b,QObject *parent = nullptr);

private:
    int a=100; //长轴
    int b=150; //短轴
    QVector2D center=QVector2D(200,200);

signals:

public slots:
    void setA(int a);
    void setB(int b);
    void setCenter(QVector2D pos);
    virtual void drawPolygon(QPainter* painter, EllipseEngine* engine);

};

#endif // ELLIPSE_H
