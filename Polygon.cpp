#include "Polygon.h"
#include <QPair>
#include <qdebug.h>
#include <QtGlobal>
#include <QtMath>

Polygon::Polygon(QObject *parent)
    : QObject{parent}
{
}

QMatrix3x3 Polygon::getTransformMatrix()
{
    QMatrix3x3 m0,m,m4;
    m0.setToIdentity();m4.setToIdentity();m.setToIdentity();
    //获取未变换的坐标中心
    QVector2D center = BoundboxInterface::getCenter();  //获取未变换时候的包围盒中中心坐标
    //将中心平移到坐标原点
    m0(0,2)=-center.x();
    m0(1,2)=-center.y();
    //获取本应有的变换矩阵
    m=TransformInterface::getTransformMatrix();
    //再平移回去
    m4(0,2)=center.x();
    m4(1,2)=center.y();
    return m4* m * m0;
}

void Polygon::setPosition(QVector2D pos)
{
    //qDebug()<<"BoundboxInterface::getCenter():"<<BoundboxInterface::getCenter();
    TransformInterface::position = pos - BoundboxInterface::getCenter();  //减去包围盒中点坐标（没变换时候的中点），得到Position好像不需要减了吧
}

QVector<QVector2D> Polygon::getBoxVerticesTransformed()
{
    return TransformInterface::getVerticesTransformed(BoundboxInterface::getBoxVertices());
}

bool Polygon::getSelected()
{
    return this->selected;
}

void Polygon::setSelected(bool state)
{
    this->selected = state;
}

bool Polygon::getFillByScan()
{
    return this->fillByScan;
}

void Polygon::setFillByScan(bool state)
{
    this->fillByScan=state;
}

bool Polygon::getFillByFence()
{
    return this->fillByFence;
}

void Polygon::setFillByFence(bool state)
{
    this->fillByFence=state;
}

void Polygon::addVertex(QVector2D vertex)
{
    this->vertices.push_back(vertex);
    //更新包围盒
    BoundboxInterface::updateBoundBox(vertex);
}

void Polygon::drawBoundBox(QPainter *painter, LineEngine* engine)
{
    //获取变换后的包围盒坐标
    QVector<QVector2D> vert = getVerticesTransformed(getBoxVertices());

    //左边
    engine->setLine(vert[0],vert[1]);
    engine->draw(painter);
    //下边
    engine->setLine(vert[1],vert[2]);
    engine->draw(painter);
    //右边
    engine->setLine(vert[2],vert[3]);
    engine->draw(painter);
    //上边
    engine->setLine(vert[3],vert[0]);
    engine->draw(painter);
    //恢复设置--不然会有bug
    engine->setLine(vert[0],vert[0]);
}

Boundbox Polygon::generateBoundBox()
{
    Boundbox box;
    //找到vertices中x.y的最大最小值
    QVector<QVector2D> vert =  getVerticesTransformed(getBoxVertices());
    if(vert.size()<1){
        //qDebug()<<"failed to generate boundBox";
        return box;
    }
    float xMin=vert[0].x();
    float xMax=vert[0].x();
    float yMin=vert[0].y();
    float yMax=vert[0].y();
    for(auto it=vert.begin(); it!=vert.end(); it++){
        QVector2D p = *it;
        xMax = xMax > p.x() ? xMax : p.x();
        xMin = xMin < p.x() ? xMin : p.x();
        yMax = yMax > p.y() ? yMax : p.y();
        yMin = yMin < p.y() ? yMin : p.y();
    }
    xMax+=1;
    xMin-=1;
    yMax+=1;
    yMin-=1;
    //更新最大最小值的点
    box.leftUp=QVector2D(xMin,yMin);
    box.leftDown=QVector2D(xMin,yMax);
    box.rightUp=QVector2D(xMax,yMin);
    box.rightDown=QVector2D(xMax,yMax);
    return box;
}

void Polygon::drawPolygon(QPainter *painter, LineEngine *engine)
{
    engine->draw(painter);
    //qDebug()<<vertices;
    QVector<QVector2D> vert = TransformInterface::getVerticesTransformed(this->vertices);
    if(vert.size()<2){
        return;
    }
    vert.push_back(vert.front());  //补齐首尾
    //对所有点利用LineEngine依次绘制
    for(auto it=(vert.begin()+1); it!=vert.end() ;it++){
        engine->setLine(*(it-1),*it);
        engine->draw(painter);
    }
    engine->setLine(QVector2D(0,0),QVector2D(0,0));
}

QVector<QVector2D> Polygon::getPolygonVerticesTransformed()
{
    return getVerticesTransformed(this->vertices);
}

QMap<int, QList<Polygon::Edge> > Polygon::gengerateET()
{
    int verticeNum = vertices.size();
    QVector<QVector2D> vert = TransformInterface::getVerticesTransformed(this->vertices);  //获取变换后的Vertices
    QMap<int, QList<Polygon::Edge> > ET;
    //顶点个数小于2，直接返回
    if(this->vertices.size()<2){
        return ET;
    }
    //维护一个offset表，标识哪一条边需要缩减
    /*
    offset[i]标识以i为起点的边，从 vert[i%verticeNum] 到 vert[(i+1)%verticeNum]
    0x00 : 不缩减
    0x01 : 缩减起点
    0x10 : 缩减终点
    0x11 : 不生成边
    Q1 : y相等的情况
    Q2 : 重复的影响
    */
    QVector<unsigned char> offset; //offset.resize(verticeNum); //有多少顶点，就有多少边
    QVector<bool> vertOffset;  //记录Vertice是否被缩减
    for(int i=0;i<verticeNum;i++){
        offset.push_back(0x00);  //默认不需要缩减
        vertOffset.push_back(false);  //初始未缩减
    }

    for(int i=0; i<verticeNum; i++){
        QVector2D last = vert[(i-1+verticeNum)%verticeNum];
        QVector2D start=vert[i];
        QVector2D end=vert[(i+1)%verticeNum];
        end.setX(round(end.x())); end.setY(round(end.y()));
        start.setX(round(start.x())); start.setY(round(start.y()));
        last.setX(round(last.x())); last.setY(round(last.y()));


        if(start.y()==end.y()){
            //先排除所有横边
            offset[i]=0x11;
        }
        //qDebug()<<"Before Edge:"<<last-QVector2D(200,200)<<start-QVector2D(200,200)<<end-QVector2D(200,200);
        if(last.y()>=start.y() && start.y() > end.y()){
            //缩减上面的边 ：Edge(start -> end)的起点，即start点
            //先判断该点是否被缩减过
            if(vertOffset[i]==false){
                offset[i] = 0x01;
                vertOffset[i]=true;
            }
        }
        else if(last.y()<start.y() && start.y() <= end.y()){
            //缩减上面的边 ：Edge(last -> start)的终点，即start点
            if(vertOffset[i]==false){
                offset[(i-1+verticeNum)%verticeNum] = 0x10;
                vertOffset[i]=true;
            }
        }
    }
//    qDebug()<<offset;
//    qDebug()<<vertOffset;
    //遍历一次顶点，根据offset，生成Et
    for(int i=0; i<verticeNum; i++){
        QVector2D start=vert[i];
        QVector2D end=vert[(i+1)%verticeNum];
        end.setX(round(end.x())); end.setY(round(end.y()));
        start.setX(round(start.x())); start.setY(round(start.y()));
        Edge edge;
        if(offset[i]==0x11){
            //edge = Edge(start,end);
            continue;
            //qDebug()<<"After Edge:"<<start-QVector2D(200,200)<<end-QVector2D(200,200);
        }else if(offset[i]==0x00){
            edge = Edge(start,end);
            //qDebug()<<"After Edge:"<<start-QVector2D(200,200)<<end-QVector2D(200,200);
        }else if(offset[i]==0x01){
            edge = Edge(QVector2D(start.x(),start.y()-1),end);
            //qDebug()<<"After Edge:"<<QVector2D(start.x(),start.y()-1)-QVector2D(200,200)<<end-QVector2D(200,200);
        }else if(offset[i]==0x10){
            edge = Edge(start,QVector2D(end.x(),end.y()-1));
            //qDebug()<<"After Edge:"<<start-QVector2D(200,200)<<QVector2D(end.x(),end.y()-1)-QVector2D(200,200);
        }else{
            //qDebug()<<"ERROR::Polygon::gengerateET : undefine offset";
        }

        //Edge edge = Edge(start,end);
        int index = start.y()<end.y() ? start.y() : end.y();  //较小的y为Index，按照ymin桶排序
        if(ET.contains(index)){
            //向链表后添加Edge——按照xmin递增排序
            int pos=0;
            for(auto it=ET[index].begin();it!=ET[index].end();it++){
                if(it->xmin > edge.xmin){
                    break;
                }
                pos++;
            }
            if(pos<ET[index].size()-1){
                ET[index].insert(pos,edge);
            }else{
                ET[index].append(edge);
            }

        }else{
            //新建链表
            QList<Edge> list;
            list.append(edge);
            ET.insert(index,list);
        }
    }
    return ET;
}

QVector<Polygon::SimpleEdge> Polygon::generateEdges()
{
    int verticeNum=vertices.size();
    QVector<QVector2D> vert = TransformInterface::getVerticesTransformed(this->vertices);  //获取变换后的Vertices
    QVector<SimpleEdge> edges;
    if(verticeNum<2){
        return edges;
    }
    for(int i=0;i<verticeNum;i++){
        QVector2D start=vert[i];
        QVector2D end=vert[(i+1)%verticeNum];
        if(start.y()-end.y()==0){
            //qDebug()<<"drop edge y=0";
            continue;
        }
        edges.push_back(SimpleEdge(start,end));
    }
    return edges;
}

int Polygon::SimpleEdge::getYmin()
{
    return start.y() < end.y() ? start.y() : end.y();
}

int Polygon::SimpleEdge::getYmax()
{
    return start.y() > end.y() ? start.y() : end.y();
}

QVector2D Polygon::SimpleEdge::getPoint(int y)
{
    //计算y值对应的线段上的点
    float t=(start.y()-y)/(start.y()-end.y());
    QVector2D point = (1-t)*start+t*end;
    point.setX(round(point.x()));
    point.setY(round(point.y()));
    return point;
}


