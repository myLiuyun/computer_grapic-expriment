#include "TransformInterface.h"

TransformInterface::TransformInterface()
{

}

TransformInterface::TransformInterface(QVector2D pos, QVector2D scale, QVector2D zoom, float angle)
{
    this->position=pos;
    this->anchor=pos;
    this->scale=scale;
    this->zoom=zoom;
    this->angle=angle;
}

QMatrix3x3 TransformInterface::getTransformMatrix()
{
    QMatrix3x3 m1,m2,m3,m4;
    m1.setToIdentity();m2.setToIdentity();m3.setToIdentity();m4.setToIdentity();
    //m1为位移矩阵
    m1(0,2) = this->position.x();
    m1(1,2) = this->position.y();
    //m2为旋转矩阵
    float rot = angle * (float)M_PI/180.0;
    m2(0,0) = cos(rot); m2(0,1)=-sin(rot);
    m2(1,0) = sin(rot); m2(1,1)=cos(rot);
    //m3为缩放矩阵
    m3(0,0) = this->scale.x(); m3(1,1)=this->scale.y();
    //m4为错切矩阵
    m4(0,1) = this->zoom.x();
    m4(1,0) = this->zoom.y();
    //更新Trasnform，先缩放，再旋转，最后平移
    return m1 * m2 * m3 * m4;
}

QVector<QVector2D> TransformInterface::getVerticesTransformed(const QVector<QVector2D>& vertices)
{
    QMatrix3x3 transform = getTransformMatrix();
    QVector<QVector2D> ans;
    for(auto it=vertices.begin();it!=vertices.end();it++){
        QVector3D temp = QVector3D(*it,1.0);
        //矩阵相乘
        temp = QVector3D(
            transform(0,0)*temp.x()+transform(0,1)*temp.y()+transform(0,2)*temp.z(),
            transform(1,0)*temp.x()+transform(1,1)*temp.y()+transform(1,2)*temp.z(),
            transform(2,0)*temp.x()+transform(2,1)*temp.y()+transform(2,2)*temp.z());
        //变回二维向量
        ans.push_back(QVector2D(temp.x()/temp.z(),temp.y()/temp.z()));
    }
    return ans;
}

void TransformInterface::setAngle(float angle)
{
    this->angle=angle;
}

void TransformInterface::setScale(float k)
{
    this->setScale(QVector2D(k,k));
}

void TransformInterface::setScale(QVector2D scale)
{
    this->scale = scale;
}

void TransformInterface::setScale(float xScale, float yScale)
{
    this->setScale(QVector2D(xScale,yScale));
}

void TransformInterface::setZoom(float x, float y)
{
    this->zoom=QVector2D(x,y);
}


void TransformInterface::addAngle(float angle)
{
    float temp=this->angle + angle;
    while(temp>360){
        temp-=360;
    }
    this->angle= temp;
}

void TransformInterface::addScale(QVector2D scale)
{
    this->scale = this->scale * scale;
}

void TransformInterface::addZoom(QVector2D zoom)
{
    this->zoom = this->zoom + zoom;
    qDebug()<<"zoom="<<zoom;
}

float TransformInterface::getAngle()
{
    return this->angle;
}

QVector2D TransformInterface::getScale()
{
    return this->scale;
}
