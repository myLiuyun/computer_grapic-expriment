#ifndef TRANSFORMINTERFACE_H
#define TRANSFORMINTERFACE_H

#include <Qmatrix3x3>
#include <QVector2D>
#include "BoundboxInterface.h"

class TransformInterface
{
public:
    TransformInterface();
    TransformInterface(QVector2D pos, QVector2D scale, QVector2D zoom, float angle);

    virtual QVector<QVector2D> getVerticesTransformed(const QVector<QVector2D>& vertices);  //返回施加变换后的一系列顶点坐标

protected:
    virtual QMatrix3x3 getTransformMatrix();  //根据中心坐标，返回变换矩阵，需要物体 未变换时 的中心坐标，子类具体实现
    QVector2D position=QVector2D(0,0);  // （position+图形中心坐标）表示和原点的偏移量，由于图形本身有偏移量，设置时要用目标位置坐标减去本身的偏移
private:
    QVector2D scale=QVector2D(1.0f,1.0f);
    QVector2D zoom=QVector2D(0,0);
    QVector2D anchor=QVector2D(0,0); //锚点，初始在图形中心
    float angle=0.0f;


public slots:
    virtual void setPosition(QVector2D pos)=0;  //每次设置position之前，需要减去没有变换的时候的 Polygon 的中心坐标
    virtual void setAngle(float angle);
    virtual void setScale(float k);
    virtual void setScale(QVector2D scale);
    virtual void setScale(float xScale,float yScale);
    virtual void setZoom(float x, float y);
    virtual void addAngle(float angle);
    virtual void addScale(QVector2D scale); //在已有基础上，施加scale
    virtual void addZoom(QVector2D zoom);
    float getAngle();

    QVector2D getScale();

};

#endif // TRANSFORMINTERFACE_H
