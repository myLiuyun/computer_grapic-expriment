#ifndef HOLEPOLYGON_H
#define HOLEPOLYGON_H

#include "Polygon.h"



class HolePolygon : public Polygon
{

public:
    explicit HolePolygon(QObject *parent = nullptr);

    virtual QMap<int,QList<Edge> > gengerateET();  //生成ET，返回生成的ET
    //virtual QVector<SimpleEdge> generateEdges();   //返回所有Edge组成的QVector，剔除斜率为0的Edge


private:
    QVector<Polygon*> holes;   //洞的边界也是多边形


public slots:
    void addHole(Polygon* polygon);
    //virtual QVector<QVector2D> getVerticesTransformed(const QVector<QVector2D>& vertices);
    virtual void setPosition(QVector2D pos);
    virtual void setAngle(float angle);
    virtual void setScale(float k);
    virtual void setScale(QVector2D scale);
    virtual void setScale(float xScale,float yScale);
    virtual void setZoom(float x, float y);
    virtual void addAngle(float angle);
    virtual void addScale(QVector2D scale); //在已有基础上，施加scale
    virtual void addZoom(QVector2D zoom);

};

#endif // HOLEPOLYGON_H
