#include "CircleEngine.h"

CircleEngine::CircleEngine(QObject *parent)
    : DrawEngine{parent}
{
    this->pen = new QPen();
    pen->setWidth(this->width);
    pen->setColor(QColor(this->color.x(),this->color.y(),this->color.z()));
}

void CircleEngine::draw(QPainter *painter)
{
    DrawEngine::draw(painter);

    QVector<QPointF> points = samplePoints();
    painter->drawPoints(points.data(),points.size());
}

QVector<QPointF> CircleEngine::samplePoints()
{
    //先假设圆心为(0,0)，计算完成后，给所有采样点坐标加一个偏置，移动到center
    QVector<QPointF> points;
    int x=0;
    int y=this->radians;
    int p=3-2*radians;
    for(; x<=y;x++){
        //坐标对称八次，得到8个采样点
        QPointF p1(x,y); QPointF p2(-x,y); QPointF p3(x,-y); QPointF p4(-x,-y);
        QPointF p5(y,x); QPointF p6(-y,x); QPointF p7(y,-x); QPointF p8(-y,-x);
        points.push_back(p1); points.push_back(p2); points.push_back(p3); points.push_back(p4);
        points.push_back(p5); points.push_back(p6); points.push_back(p7); points.push_back(p8);
        //更新判别量Pi
        if(p>=0){
            p+=4*(x-y)+10;
            y--;
        }else{
            p+=4*x+6;
        }
    }
    //坐标偏置
    for(auto it=points.begin(); it!=points.end();it++){
        QPointF p=*it;
        *it = QPointF(p.x()+center.x(),p.y()+center.y());
    }
    return points;
}

void CircleEngine::setRadians(int rand)
{
    this->radians=rand;
}

void CircleEngine::setCenter(QVector2D pos)
{
    this->center=pos;
}
