#ifndef LINEENGINE_H
#define LINEENGINE_H

#include <QWidget>
#include <QVector2D>
#include <QPainter>
#include <QPaintEvent>
#include <QPaintDevice>
#include <drawengine.h>

class LineEngine : public DrawEngine
{
    Q_OBJECT
public:
    explicit LineEngine(QObject *parent = nullptr);  //制定绘图设备

protected:
    QVector<QPointF> bresenham();   //使用Bresenham算法，采样要绘制的像素点，返回所有点
    QVector2D start=QVector2D(0,0);
    QVector2D end=QVector2D(0,0);

private:

    void change(unsigned char type);
    void inverseChange(QVector<QPointF>& points, unsigned char type);
signals:


public slots:
    virtual void draw(QPainter* painter);  // 调用该方法，绘制直线，使用bresenham方法采样，加Virtual 才有多态
    void setStart(QVector2D start);
    void setEnd(QVector2D end);
    void setLine(QVector2D start,QVector2D end);
};

#endif // LINEENGINE_H
