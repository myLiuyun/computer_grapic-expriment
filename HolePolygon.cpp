#include "HolePolygon.h"

HolePolygon::HolePolygon(QObject *parent)
    : Polygon{parent}
{

}

QMap<int, QList<Polygon::Edge> > HolePolygon::gengerateET()
{
    QMap<int, QList<Polygon::Edge> > ET = Polygon::gengerateET();
    //将Hole的Edge也加入ET
    //for(auto polygon = holes.begin())

    for(auto it=holes.begin();it!=holes.end();it++){
        //获取每一个洞的ET
        QMap<int, QList<Polygon::Edge> > ET2 = (*it)->gengerateET();
        //对每一个ET项，加入ET
        for(auto edge = ET2.begin(); edge!=ET2.end(); edge++){
            if(ET.contains(edge.key())){
                QList<Polygon::Edge> items = edge.value();
                for(auto e = items.begin(); e!=items.end();e++){
                    ET[edge.key()].push_back(*e);
                }
            }else{
                QList<Polygon::Edge> items = edge.value();
                ET.insert(edge.key(),edge.value());
            }
        }
    }
    /*输出一下ET*/
    qDebug()<<"-----------------ET---------------------";
    for(auto it=ET.begin();it!=ET.end();it++){
        qDebug()<<it.key();
        for(auto edge=it->begin();edge!=it->end();edge++){
            qDebug()<<edge->ymax<<" "<<edge->xmin<<" "<<(float)edge->k;
        }
    }
    qDebug()<<"-----------------ET---------------------";
    return ET;
}

void HolePolygon::addHole(Polygon *polygon)
{
    this->holes.push_back(polygon);
}

void HolePolygon::setPosition(QVector2D pos)
{
    Polygon::setPosition(pos);
    for(auto it=holes.begin();it!=holes.end();it++){
        (*it)->setPosition(pos);
    }
}

void HolePolygon::setAngle(float angle)
{
    TransformInterface::setAngle(angle);
    for(auto it=holes.begin();it!=holes.end();it++){
        (*it)->setAngle(angle);
    }
}

void HolePolygon::setScale(float k)
{
    TransformInterface::setScale(k);
    for(auto it=holes.begin();it!=holes.end();it++){
        (*it)->setScale(k);
    }
}

void HolePolygon::setScale(QVector2D scale)
{
    TransformInterface::setScale(scale);
    for(auto it=holes.begin();it!=holes.end();it++){
        (*it)->setScale(scale);
    }
}

void HolePolygon::setScale(float xScale, float yScale)
{
    TransformInterface::setScale(QVector2D(xScale,yScale));
    for(auto it=holes.begin();it!=holes.end();it++){
        (*it)->setScale(QVector2D(xScale,yScale));
    }
}

void HolePolygon::setZoom(float x, float y)
{
    TransformInterface::setZoom(x,y);
    for(auto it=holes.begin();it!=holes.end();it++){
        (*it)->setZoom(x,y);
    }
}

void HolePolygon::addAngle(float angle)
{
    TransformInterface::addAngle(angle);
    for(auto it=holes.begin();it!=holes.end();it++){
        (*it)->addAngle(angle);
    }
}

void HolePolygon::addScale(QVector2D scale)
{
    TransformInterface::addScale(scale);
    for(auto it=holes.begin();it!=holes.end();it++){
        (*it)->addScale(scale);
    }
}

void HolePolygon::addZoom(QVector2D zoom)
{
    TransformInterface::addZoom(zoom);
    for(auto it=holes.begin();it!=holes.end();it++){
        (*it)->addZoom(zoom);
    }
}


//void HolePolygon::addHole(Polygon* polygon)
//{
//    holes.push_back(*polygon);
//}

