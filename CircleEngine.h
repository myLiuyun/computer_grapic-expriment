#ifndef CIRCLEENGINE_H
#define CIRCLEENGINE_H

#include <QObject>
#include <QPainter>
#include <QVector2D>
#include <drawengine.h>

class CircleEngine : public DrawEngine
{
    Q_OBJECT
public:
    int radians=100;
    QVector2D center = QVector2D(100,100);

    explicit CircleEngine(QObject *parent = nullptr);
    void draw(QPainter* painter) override;

private:
    QVector<QPointF> samplePoints();   //使用圆的Bresenham扫描转换算法，计算采样点

signals:

public slots:
    void setRadians(int rand);
    void setCenter(QVector2D pos);

};

#endif // CIRCLEENGINE_H
