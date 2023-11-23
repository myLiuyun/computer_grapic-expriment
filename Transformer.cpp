#include "Transformer.h"
#include <QVector2D>
#include <math.h>

Transformer::Transformer(QObject *parent)
    : QObject{parent}
{
    this->transform.setToIdentity();
}

QMatrix3x3 Transformer::getTransform()
{
    return this->transform;
}

void Transformer::addMovement(const QVector2D &pos)
{
    QMatrix3x3 moveMat;
    moveMat.setToIdentity();
    moveMat(0,2)=pos.x();
    moveMat(1,2)=pos.y();
    this->transform = moveMat * transform;
}

void Transformer::addRotation(const QVector2D& anchor, float angle)
{
    //计算变换后的锚点
    QVector3D temp=QVector3D(anchor,1.0f);
    temp = QVector3D(
        transform(0,0)*temp.x()+transform(0,1)*temp.y()+transform(0,2)*temp.z(),
        transform(1,0)*temp.x()+transform(1,1)*temp.y()+transform(1,2)*temp.z(),
        transform(2,0)*temp.x()+transform(2,1)*temp.y()+transform(2,2)*temp.z());
    //变回二维向量
    QVector2D an = QVector2D(temp.x()/temp.z(), temp.y()/temp.z());

    QMatrix3x3 m1,m2,m3;
    m1.setToIdentity(); m2.setToIdentity(); m3.setToIdentity();
    m1(0,2)=an.x();  m1(1,2)=an.y();
    m3(0,2)=-an.x(); m3(1,2)=-an.y();
    //角度转弧度
    angle = angle * (float)M_PI/180.0;
    m2(0,0)=cos(angle); m2(0,1)=-sin(angle);
    m2(1,0)=sin(angle); m2(1,1)=cos(angle);

    this->transform = m1 * m2 * m3 * transform;
}

void Transformer::addScale(const QVector2D& anchor, float x, float y)
{
    //计算变换后的锚点
    QVector3D temp=QVector3D(anchor,1.0f);
    temp = QVector3D(
        transform(0,0)*temp.x()+transform(0,1)*temp.y()+transform(0,2)*temp.z(),
        transform(1,0)*temp.x()+transform(1,1)*temp.y()+transform(1,2)*temp.z(),
        transform(2,0)*temp.x()+transform(2,1)*temp.y()+transform(2,2)*temp.z());
    //变回二维向量
    QVector2D an = QVector2D(temp.x()/temp.z(), temp.y()/temp.z());
    //计算变换矩阵
    QMatrix3x3 m1,m2,m3;
    m1.setToIdentity(); m2.setToIdentity(); m3.setToIdentity();
    m1(0,2)=an.x();  m1(1,2)=an.y();
    m3(0,2)=-an.x(); m3(1,2)=-an.y();

    m2.setToIdentity();
    m2(0,0)=x; m2(1,1)=y;
    this->transform = m1 * m2 * m3 * this->transform;
}

void Transformer::addScale(const QVector2D& anchor, float k)
{
    this->addScale(anchor,k,k);
}
