/*
多边形类，存储多边形的属性，包括：
1、顶点
2、边表
*/
#ifndef POLYGON_H
#define POLYGON_H

#include <QObject>
#include <QVector2D>
#include <QMap>
#include <QList>
#include <QDebug>
#include <QGenericMatrix>
#include <QPainter>
#include "LineEngine.h"
#include "TransformInterface.h"
#include "BoundboxInterface.h"

class Polygon : public QObject, public TransformInterface, public BoundboxInterface
{
    Q_OBJECT
public:
    struct Edge{
        int ymax;
        float xmin;
        float k;  //斜率的倒数
        Edge(){}
        Edge(QVector2D start, QVector2D end){
            this->ymax = start.y()>end.y() ? start.y() : end.y();
            this->xmin = start.y()<end.y() ? (float)start.x() : (float)end.x();  //x对应扫描线交点的初值，对应y比较小的端点对应的x值
            this->k = (start.x()-end.x())/(start.y()-end.y());
            if(start.y()==end.y()){
                this->k=100000;
            }
        }
        // 实现小于运算符，以便结构体可比较
        bool operator<(const Edge& other) const {
            // 按照xmin升序
            return this->xmin < other.xmin;
        }
    };
    struct SimpleEdge{
        QVector2D start;
        QVector2D end;
        SimpleEdge(QVector2D start, QVector2D end){
            this->start=start;
            this->end=end;
        }
        int getYmin();
        int getYmax();
        QVector2D getPoint(int y);
    };

    explicit Polygon(QObject *parent = nullptr);

protected:

    QVector<QVector2D> vertices;   //顶点
    bool selected=false;
    bool fillByScan=false;
    bool fillByFence=false;
private:


public slots:
    virtual void drawPolygon(QPainter* painter, LineEngine* engine);  //绘制多边形
    virtual QVector<QVector2D> getPolygonVerticesTransformed();  //获取变换后的多边形顶点坐标
    virtual QMatrix3x3 getTransformMatrix() override;  //获取变换矩阵
    virtual void setPosition(QVector2D pos) override;  //设置Transform中的position
    virtual QVector<QVector2D> getBoxVerticesTransformed() override;  //获取变换后的包围盒坐标
    virtual void drawBoundBox(QPainter* painter, LineEngine* engine) override;  //绘制包围盒
    virtual Boundbox generateBoundBox() override;


    bool getSelected();
    void setSelected(bool state);
    bool getFillByScan();
    void setFillByScan(bool state);
    bool getFillByFence();
    void setFillByFence(bool state);
    void addVertex(QVector2D vertex);      // 添加顶点

    virtual QMap<int,QList<Edge> > gengerateET();  //生成ET，返回生成的ET
    virtual QVector<SimpleEdge> generateEdges();   //返回所有Edge组成的QVector，剔除斜率为0的Edge
signals:

};

#endif // POLYGON_H
