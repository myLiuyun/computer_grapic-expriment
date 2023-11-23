#include "StarPolygon.h"

StarPolygon::StarPolygon(int size , QVector2D position, float rotate, QObject *parent)
    : Polygon{parent}
{
    setPosition(position);
    setScale(size);
    setAngle(rotate);
//    this->position=position;
//    this->angle=rotate;
//    this->scale=QVector2D(size,size);
    pentagram(1,0,0,0); //生成一个1*1范围内的Star，保存为原始顶点
}

/*
int R:五角星的长轴
int x, y:五角星的中心点
int yDegree:与y轴最近的长轴与y轴的夹角，
*/
void StarPolygon::pentagram(int R, int x, int y, int yDegree)
{
    double rad = 3.1415926 / 180;                    //每度的弧度值
    double r = R * sin(18 * rad) / cos(36 * rad);    //五角星短轴的长度
    QVector2D RVertex[5], rVertex[5];
    for (int k = 0; k < 5; k++)                      //求取坐标
    {
        RVertex[k] = QVector2D( x - (R * cos((90 + k * 72 + yDegree) *rad)), y - (R * sin((90 + k * 72 + yDegree) * rad) ) );
        rVertex[k] = QVector2D( x - (r * cos((90 + 36 + k * 72 + yDegree) *rad)), y - (r * sin((90 + 36 + k * 72 + yDegree) * rad)) );
    }
    for(int i=0;i<5;i++){
        addVertex(RVertex[i]);
        addVertex(rVertex[i]);
    }
}

void StarPolygon::drawPolygon(QPainter *painter, LineEngine *engine)
{
//    qDebug()<<"star::drawPolygon";
//    qDebug()<<vertices;
//    qDebug()<<getVerticesTransformed(vertices);
    Polygon::drawPolygon(painter,engine);
}
