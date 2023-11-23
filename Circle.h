#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include <QVector2D>
#include <QPainter>

#include "CircleEngine.h"

class Circle : public QObject
{
    Q_OBJECT
public:
    explicit Circle(QObject *parent = nullptr);
    Circle(int randians, QVector2D center);
private:
    int radians=100;
    QVector2D center = QVector2D(100,100);

signals:

public slots:
    virtual void drawPolygon(QPainter* painter, CircleEngine* engine);
    void setRadians(float rand);
    void setCenter(QVector2D pos);
};

#endif // CIRCLE_H
