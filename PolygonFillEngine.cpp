#include "PolygonFillEngine.h"
#include <QSet>
#include <QDebug>
#include <math.h>


PolygonFillEngine::PolygonFillEngine(QObject *parent)
    : DrawEngine{parent}
{

}

void PolygonFillEngine::draw(QPainter *painter)
{
    qDebug()<<"failed to use";
}

void PolygonFillEngine::scanpolygonFill(Polygon *polygon, QPainter *painter)
{
    DrawEngine::draw(painter);
    Boundbox box = polygon->generateBoundBox();

    QMap<int, QList<Polygon::Edge> > ET = polygon->gengerateET();  //边表ET
    QVector<Polygon::Edge> AET; //活跃边表


    //生成AET
    int yMin = box.getYMin();
    int yMax = box.getYMax();
    //从yMin到t=yMax扫描
    for(int y=yMin-1; y<=yMax+1 ; y++){
        if(ET.contains(y)){
            //有新的Edge,加入到AET
            QList<Polygon::Edge> list = ET[y];
            for(auto it=list.begin(); it!=list.end(); it++){
                AET.push_back(*it);
            }
        }
        //对AET按照x值排序
        std::sort(AET.begin(),AET.end());
        //奇偶填充
        //qDebug()<<"y="<<y<<" Size = "<<AET.size();
        if(AET.size()%2!=0){
            //qDebug()<<"y="<<y<<" error::AET.size() %2 != 0. Size = "<<AET.size();
            //更新AET，更新x值，清除已经结束的Edge
            for(auto edge=AET.begin(); edge!=AET.end();){
                //更新Edge中的X值
                edge->xmin=edge->xmin + edge->k;
                //清除y=ymax的Edge
                if(edge->ymax<=y){
                    //qDebug()<<"earse edge::ymax="<<edge->ymax;
                    edge=AET.erase(edge);

                }else{
                    edge++;
                }
            }
            continue;
        }
        for(auto edge=AET.begin(); edge!=AET.end(); edge+=2){
            //填充一条直线,填充两Edge交点之间的直线
            painter->drawLine(QPoint(round(edge->xmin),y),QPoint(round((edge+1)->xmin),y));  //这里用了一次drawLine
        }
        //更新AET，更新x值，清除已经结束的Edge
        for(auto edge=AET.begin(); edge!=AET.end();){
            //更新Edge中的X值
            edge->xmin=edge->xmin + edge->k;
            //清除y=ymax的Edge
            if(edge->ymax<=y){
                //qDebug()<<"earse edge::ymax="<<edge->ymax;
                edge=AET.erase(edge);

            }else{
                edge++;
            }
        }
    }
}

void PolygonFillEngine::fenceFill(Polygon *polygon, QPainter *painter)
{
    DrawEngine::draw(painter);

    QVector<Polygon::SimpleEdge> edges = polygon->generateEdges();  //所有与Y不平行的Edge，变换后的
    QVector<QPointF> points;
    //初始化Mask
    QVector<QVector<bool> > mask;
    Boundbox box = polygon->generateBoundBox();
    for(int i=0; i<box.getXMax()-box.getXMin()+3; i++){
        QVector<bool> item;
        item.resize(box.getYMax()-box.getYMin()+3);
        for(int j=0; j<box.getYMax()-box.getYMin()+3; j++){
            item[j]=false;
        }
        mask.push_back(item);
    }
    qDebug()<<"inital mask";
    // 选一个栅栏，需要过多边形某个顶点
    int fence=box.getXMax()/2+box.getXMin()/2;
    for(auto edge=edges.begin(); edge!=edges.end();edge++){
    //取出一条边
        for(int y=edge->getYmin();y<edge->getYmax();y++){
        //扫描这条边，计算交点
            QVector2D point = edge->getPoint(y);
            if(point.x()<box.getXMin()) point.setX(box.getXMin()+1);
            if(point.y()<box.getYMin()) point.setY(box.getYMin()+1);
            if(point.x()>box.getXMax()) point.setX(box.getXMax()-1);
            if(point.y()>box.getYMax()) point.setY(box.getYMax()-1);
            //改变Mask
            if(point.x()<fence){
                //交点在栅栏左侧
                for(int x=point.x(); x<fence;x++){
                    mask[x-box.getXMin()][y-box.getYMin()]=!mask[x-box.getXMin()][y-box.getYMin()];
                }
            }else{
                //交点在栅栏右边
                if(point.x()>box.getXMax()+2){
                    qDebug()<<"ERROR:point.x()>box.getXMax()";
                    qDebug()<<box.getXMin()<<box.getXMax()<<box.getYMin()<<box.getYMax();
                    qDebug()<<edge->start<<edge->end;
                    qDebug()<<edge->getYmax()<<edge->getYmin();
                    qDebug()<<y<<point;
                    continue;
                }
                for(int x=fence; x<point.x();x++){
                    mask[x-box.getXMin()][y-box.getYMin()]=!mask[x-box.getXMin()][y-box.getYMin()];
                }
            }
        }
    }
    //根据Mask，填充像素
    for(int i=0; i<box.getXMax()-box.getXMin()+1; i++){
        for(int j=0; j<box.getYMax()-box.getYMin()+1; j++){
            if(mask[i][j]){
                points.push_back(QPointF(box.getXMin()+i,box.getYMin()+j));
            }
        }
    }
    //绘制点
    painter->setPen(*this->pen);
    painter->drawPoints(points.data(),points.size());

}
