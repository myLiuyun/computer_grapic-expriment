#include "PolygonEngine.h"

PolygonEngine::PolygonEngine(Polygon *polygon, QObject *parent)
    : LineEngine{parent}
{
    this->polygon=polygon;
}

void PolygonEngine::draw(QPainter *painter)
{
    LineEngine::draw(painter);

    QVector<QVector2D> vertices = this->polygon->getPolygonVerticesTransformed();
    if(vertices.size()<2){
        return;
    }
    //对所有店利用LineEngine依次绘制
    for(auto it=(vertices.begin()+1); it!=vertices.end() ;it++){
        //绘制每一条直线
        //qDebug()<<*(it-1)<<*it;
        LineEngine::start=*(it-1);
        LineEngine::end=*(it);
        LineEngine::draw(painter);
    }
    LineEngine::start=vertices.back();
    LineEngine::end=vertices.front();
    //qDebug()<<start<<end;
    LineEngine::draw(painter);
}

void PolygonEngine::setPolygon(Polygon *polygon)
{
    this->polygon = polygon;
}
