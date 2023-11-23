#include "BoundboxInterface.h"
#include <QDebug>

BoundboxInterface::BoundboxInterface()
{

}

BoundboxInterface::BoundboxInterface(const QVector<QVector2D> &vertices)
{
    if(vertices.size()<1){
        qDebug()<<"failed to generate boundBox";
        return;
    }
    float xMin=vertices[0].x();
    float xMax=vertices[0].x();
    float yMin=vertices[0].y();
    float yMax=vertices[0].y();
    for(auto it=vertices.begin(); it!=vertices.end(); it++){
        QVector2D p = *it;
        xMax = xMax > p.x() ? xMax : p.x();
        xMin = xMin < p.x() ? xMin : p.x();
        yMax = yMax > p.y() ? yMax : p.y();
        yMin = yMin < p.y() ? yMin : p.y();
    }
    //生成四个点
    this->leftUp=QVector2D(xMin,yMin);
    this->leftDown=QVector2D(xMin,yMax);
    this->rightUp=QVector2D(xMax,yMin);
    this->rightDown=QVector2D(xMax,yMax);
}

BoundboxInterface::BoundboxInterface(const QVector2D &start, const QVector2D &end)
{
    int xMin=std::min(start.x(),end.x()) - 1;
    int xMax=std::max(start.x(),end.x()) + 1;
    int yMin=std::min(start.y(),end.y()) - 1;
    int yMax=std::max(start.y(),end.y()) + 1;
    //生成四个点
    leftUp=QVector2D(xMin,yMin);
    leftDown=QVector2D(xMin,yMax);
    rightUp=QVector2D(xMax,yMin);
    rightDown=QVector2D(xMax,yMax);
}

float BoundboxInterface::getXMin()
{
    return leftUp.x();
}

float BoundboxInterface::getXMax()
{
    return rightUp.x();
}

float BoundboxInterface::getYMin()
{
    return leftUp.y();
}

float BoundboxInterface::getYMax()
{
    return leftDown.y();
}

QVector2D BoundboxInterface::getLeftUp()
{
    return leftUp;
}

QVector2D BoundboxInterface::getLeftDown()
{
    return leftDown;
}

QVector2D BoundboxInterface::getRightUp()
{
    return rightUp;
}

QVector2D BoundboxInterface::getRightDown()
{
    return rightDown;
}

QVector2D BoundboxInterface::getCenter()
{
    return (leftUp+leftDown+rightUp+rightDown)/4;
}

QVector<QVector2D> BoundboxInterface::getBoxVertices()
{
    QVector<QVector2D> vertices;
    vertices.push_back(leftUp);
    vertices.push_back(leftDown);
    vertices.push_back(rightDown);
    vertices.push_back(rightUp);

    return vertices;
}

void BoundboxInterface::updateBoundBox(const QVector<QVector2D> &vertices)
{
    //找到vertices中x.y的最大最小值
    if(vertices.size()<1){
        qDebug()<<"failed to generate boundBox";
        return;
    }
    float xMin=vertices[0].x();
    float xMax=vertices[0].x();
    float yMin=vertices[0].y();
    float yMax=vertices[0].y();
    for(auto it=vertices.begin(); it!=vertices.end(); it++){
        QVector2D p = *it;
        xMax = xMax > p.x() ? xMax : p.x();
        xMin = xMin < p.x() ? xMin : p.x();
        yMax = yMax > p.y() ? yMax : p.y();
        yMin = yMin < p.y() ? yMin : p.y();
    }
    //更新最大最小值的点
    if(xMin<this->getXMin()){
        this->leftUp.setX(xMin);
        this->leftDown.setX(xMin);
    }
    if(xMax>this->getXMax()){
        this->rightUp.setX(xMax);
        this->rightDown.setX(xMax);
    }
    if(yMin<this->getYMin()){
        this->leftUp.setY(yMin);
        this->rightUp.setY(yMin);
    }
    if(yMax>this->getYMax()){
        this->leftDown.setY(yMax);
        this->rightDown.setY(yMax);
    }
}

void BoundboxInterface::updateBoundBox(const QVector2D &vertex)
{
    float xMin=vertex.x();
    float xMax=vertex.x();
    float yMin=vertex.y();
    float yMax=vertex.y();
    //更新最大最小值的点
    if(xMin<this->getXMin()){
        this->leftUp.setX(xMin);
        this->leftDown.setX(xMin);
    }
    if(xMax>this->getXMax()){
        this->rightUp.setX(xMax);
        this->rightDown.setX(xMax);
    }
    if(yMin<this->getYMin()){
        this->leftUp.setY(yMin);
        this->rightUp.setY(yMin);
    }
    if(yMax>this->getYMax()){
        this->leftDown.setY(yMax);
        this->rightDown.setY(yMax);
    }
}


bool Boundbox::inBox(QVector2D pos)
{
    float xMin = this->leftDown.x();
    float xMax = this->rightDown.x();
    float yMin = this->leftUp.y();
    float yMax = this->leftDown.y();
    return pos.x()<=xMax && pos.x()>=xMin && pos.y()>=yMin && pos.y()<=yMax;
}

float Boundbox::getXMin()
{
    return (int)leftUp.x();
}

float Boundbox::getXMax()
{
    return (int)rightUp.x();
}

float Boundbox::getYMin()
{
    return (int)leftUp.y();
}

float Boundbox::getYMax()
{
    return (int)leftDown.y();
}

QVector2D Boundbox::getCenter()
{
    QVector2D point = (leftUp+leftDown+rightUp+rightDown)/4;
    point = QVector2D((int)point.x(),(int)point.y());
    return point;
}
