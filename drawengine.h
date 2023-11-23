#ifndef DRAWENGINE_H
#define DRAWENGINE_H

#include <QObject>
#include <QPainter>
#include <QVector3D>

class DrawEngine : public QObject
{
    Q_OBJECT
public:
    explicit DrawEngine(QObject *parent = nullptr);
    virtual void draw(QPainter* painter); // 所有子类绘制前调用此函数，设置Painter参数
    QPen* pen=nullptr;
    int width=1;
    QVector4D color=QVector4D(255,0,0,255);


signals:

public slots:
    void setColor(QVector4D color);
    void setWidth(int width);
    QVector4D getColor();
};

#endif // DRAWENGINE_H
