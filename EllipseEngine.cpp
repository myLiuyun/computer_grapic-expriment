#include "EllipseEngine.h"
#include <QVector>
#include <QDebug>

EllipseEngine::EllipseEngine(QObject *parent)
    : DrawEngine{parent}
{

}

QVector<QPointF> EllipseEngine::samplePoints()
{
    //先假设center为(0,0)
    QVector<QPointF> points;
    int x=0,y=this->b;
    double d1,d2;
    double _a=this->a,_b=this->b;

    d1=_b*_b+_a*_a*(-_b*0.25);
    //前1/8
    while(_b*_b*(x+1) < _a*_a*(y-0.5)){
        //对称四次
        QPointF p1(x,y),p2(x,-y),p3(-x,y),p4(-x,-y);
        points.push_back(p1); points.push_back(p2); points.push_back(p3); points.push_back(p4);
        if(d1<0){
            d1+=_b*_b*(2*x+3);
            x++;
        }else{
            d1+=(_b*_b*(2*x+3)+_a*_a*(-2*y+2));
            x++;
            y--;
        }
        //qDebug()<<d1;
    }
    //后1/8
    d2=_b*_b*(x+0.5)*(x+0.5)+_a*_a*(y-1)*(y-1)-_a*_a*_b*_b;
    while(y>0){
        //对称四次
        QPointF p1(x,y),p2(x,-y),p3(-x,y),p4(-x,-y);
        points.push_back(p1); points.push_back(p2); points.push_back(p3); points.push_back(p4);
        if(d2<0){
            d2+=_b*_b*(2*x+2)+_a*_a*(-2*y+3);
            x++;
            y--;
        }else{
            d2+=_a*_a*(-2*y+3);
            y--;
        }
    }
    //加上偏置
    for(auto it=points.begin();it!=points.end();it++){
        *it=*it+QPointF(center.x(),center.y());
    }
    return points;
}

void EllipseEngine::draw(QPainter *painter)
{
    DrawEngine::draw(painter);

    QVector<QPointF> points=samplePoints();  //获取直线的采样点
    painter->drawPoints(points.data(),points.size());
}
