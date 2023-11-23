#include "LineEngine.h"

LineEngine::LineEngine(QObject *parent)
    :DrawEngine(parent)
{
    //设置好画笔的各项参数
    this->pen = new QPen();
    pen->setWidth(this->width);
    pen->setColor(QColor(this->color.x(),this->color.y(),this->color.z()));
}

QVector<QPointF> LineEngine::bresenham()
{
    //运行bresenham算法，采样要绘制的点，先假设start = (0,0) , end = end-start
    //先保证直线方向为沿x轴递增
    if(end.x()<=start.x()){
        QVector2D temp=end;
        end=start;
        start=temp;
    }
    end = end-start;
    //根据四种情况，进行变换
    unsigned char type=0x00;
    if(end.y()>= 0 && end.x()>=end.y()){
        type=0x00;
    }
    else if(end.y()>= 0 && end.x()<=end.y()){
        type=0x01;  // m为1到正无穷，将end的x和y交换，计算采样点，最后交换回来
    }
    else if(end.y()<= 0 && end.x()>=abs(end.y())){
        type=0x10;  // m为-1到0，将end点沿x=0对称，直线变为递增，m属于0到1
    }
    else if(end.y()<= 0 && end.x()<=abs(end.y())){
        type=0x11;  // 先进行type=0x10的变换，m变为正的，但是>1，再进行type=0x01的变换，m变为0到1
    }
    //qDebug()<<start<<end;
    change(type);
    //qDebug()<<start<<end;
    //计算判别式
    int x,y,dx,dy,p;
    x=0; y=0;
    dx=end.x()-0; dy=end.y()-0;
    p=2*dy-dx;
    //采样
    QVector<QPointF> samplePoints;
    int step=0;
    for(;x<end.x();x++,step++){
        samplePoints.push_back(QPointF(x,y));
        if(p>0){
            y++;
            p+=2*(dy-dx);
        }else{
            p+=2*dy;
        }
    }
    //执行逆变换，得到原坐标中的采样点
    inverseChange(samplePoints,type);
    end+=start;
    //坐标变换 -- 所有采样点加上start的偏置
    for(auto it=samplePoints.begin();it!=samplePoints.end();it++){
        *it = *it + QPointF(start.x(),start.y());
    }
    //qDebug()<<start<<end;
    //qDebug()<<samplePoints;
    return samplePoints;
}

void LineEngine::change(unsigned char type)
{
    switch (type) {
    case 0x01:
        end = QVector2D(end.y(),end.x());
        break;
    case 0x10:
        end = QVector2D(end.x(),-end.y());
        break;
    case 0x11:
        end = QVector2D(end.x(),-end.y());
        end = QVector2D(end.y(),end.x());
        break;
    default:
        break;
    }
}

void LineEngine::inverseChange(QVector<QPointF> &points, unsigned char type)
{
    for(auto it=points.begin(); it!=points.end();it++){
        switch (type) {
        case 0x01:
            *it = QPointF(it->y(),it->x());  //再交换一次x,y
            break;
        case 0x10:
            *it = QPointF(it->x(),-it->y());  //重新沿着x=start.x()，将采样点对称
            break;
        case 0x11:
            *it = QPointF(it->y(),it->x());
            *it = QPointF(it->x(),-it->y());
        default:
            break;
        }
    }
    //将end也变回去
    switch (type) {
    case 0x01:
        end = QVector2D(end.y(),end.x());
        break;
    case 0x10:
        end = QVector2D(end.x(),-end.y());
        break;
    case 0x11:
        end = QVector2D(end.y(),end.x());
        end = QVector2D(end.x(),-end.y());
        break;
    default:
        break;
    }
}

void LineEngine::draw(QPainter *painter)
{
    DrawEngine::draw(painter);

    QVector<QPointF> points=bresenham();  //获取直线的采样点
    painter->drawPoints(points.data(),points.size());
}

void LineEngine::setStart(QVector2D start)
{
    this->start = start;
}

void LineEngine::setEnd(QVector2D end)
{
    this->end = end;
}

void LineEngine::setLine(QVector2D start, QVector2D end)
{
    this->start = start;
    this->end = end;
}
