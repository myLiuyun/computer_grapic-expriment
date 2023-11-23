#include "Line.h"
#include <QMatrix4x4>

Line::Line(QObject *parent)
    : QObject{parent}
{

}

Line::Line(QVector2D start, QVector2D end, QObject *parent)
    : QObject{parent}
{
    this->start=start;
    this->end=end;
}

void Line::setStart(QVector2D pos)
{
    //先逆变换，再设置
    pos=transform(invertedMatrix(getTransform()),pos);
    this->start=pos;
}

void Line::setEnd(QVector2D pos)
{
    pos=transform(invertedMatrix(getTransform()),pos);
    this->end=pos;
}

void Line::setSelected(bool state)
{
    this->selected=state;
}

bool Line::getSelected()
{
    return this->selected;
}

QVector2D Line::getStart()
{
    //根据参数，进行变换，再返回
    QVector2D p1 = transform(getTransform(),start);
    QVector2D p2 = transform(getTransform(),end);
    if(p1.x()<p2.x()){
        return p1;
    }
    return p2;
}

QVector2D Line::getEnd()
{
    //根据参数，进行变换，再返回
    QVector2D p1 = transform(getTransform(),start);
    QVector2D p2 = transform(getTransform(),end);
    if(p1.x()<p2.x()){
        return p2;
    }
    return p1;
}

Line::BoundBox Line::getBox()
{
    int xMin=std::min(getStart().x(),getEnd().x()) - 1;
    int xMax=std::max(getStart().x(),getEnd().x()) + 1;
    int yMin=std::min(getStart().y(),getEnd().y()) - 1;
    int yMax=std::max(getStart().y(),getEnd().y()) + 1;

    BoundBox box;
    box.leftUp=QVector2D(xMin,yMin);
    box.leftDown=QVector2D(xMin,yMax);
    box.rightUp=QVector2D(xMax,yMin);
    box.rightDown=QVector2D(xMax,yMax);
    return box;
}

void Line::draw(QPainter* painter, LineEngine *engine)
{
    engine->setLine(getStart(),getEnd());
    engine->draw(painter);
}

void Line::drawBox(QPainter* painter, LineEngine *engine)
{
    BoundBox box=this->getBox();
    engine->setLine(box.leftUp,box.rightUp);
    engine->draw(painter);
    engine->setLine(box.rightUp,box.rightDown);
    engine->draw(painter);
    engine->setLine(box.rightDown,box.leftDown);
    engine->draw(painter);
    engine->setLine(box.leftDown,box.leftUp);
    engine->draw(painter);
}

QMatrix3x3 Line::getTransform()
{
    //获取变换矩阵
    QMatrix3x3 m0,m1,m2,m3;
    m0.setToIdentity();m1.setToIdentity();m2.setToIdentity();m3.setToIdentity();
    //m0为错切矩阵
    m0(0,1)=this->zoom.x();
    m0(1,0)=this->zoom.y();
    //m1为位移矩阵
    m1(0,2)=this->position.x();
    m1(1,2)=this->position.y();
    //m2为旋转矩阵
    float rot = rotote * (float)M_PI/180.0;
    m2(0,0)=cos(rot); m2(0,1)=-sin(rot);
    m2(1,0)=sin(rot); m2(1,1)=cos(rot);
    //m3为缩放矩阵
    m3(0,0)=this->scale.x(); m3(1,1)=this->scale.y();
    //更新Trasnform，先缩放，再旋转，最后平移
    return m0 * m1 * m2 * m3;
}

QVector2D Line::transform(QMatrix3x3 transform, QVector2D point)
{
    QVector3D temp=QVector3D(point,1.0f);
    temp = QVector3D(
            transform(0,0)*temp.x()+transform(0,1)*temp.y()+transform(0,2)*temp.z(),
            transform(1,0)*temp.x()+transform(1,1)*temp.y()+transform(1,2)*temp.z(),
            transform(2,0)*temp.x()+transform(2,1)*temp.y()+transform(2,2)*temp.z()
        );
    return QVector2D(temp.x()/temp.z(),temp.y()/temp.z());
}

QMatrix3x3 Line::invertedMatrix(QMatrix3x3 mat)
{
    QMatrix4x4 m;
    m.setToIdentity();
    m(0,0)=mat(0,0); m(0,1)=mat(0,1); m(0,2)=mat(0,2);
    m(1,0)=mat(1,0); m(1,1)=mat(1,1); m(1,2)=mat(1,2);
    m(2,0)=mat(2,0); m(2,1)=mat(2,1); m(2,2)=mat(2,2);
    m = m.inverted();
    mat(0,0)=m(0,0); mat(0,1)=m(0,1); mat(0,2)=m(0,2);
    mat(1,0)=m(1,0); mat(1,1)=m(1,1); mat(1,2)=m(1,2);
    mat(2,0)=m(2,0); mat(2,1)=m(2,1); mat(2,2)=m(2,2);
    return mat;
}


bool Line::BoundBox::inBox(QVector2D pos)
{
    int xMax=rightDown.x();
    int xMin=leftDown.x();
    int yMax=rightDown.y();
    int yMin=rightUp.y();
    return xMax>pos.x() && xMin<pos.x() && yMax>pos.y() && yMin<pos.y();
}
