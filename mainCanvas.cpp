#include "mainCanvas.h"
#include <QPainter>
#include <QVector2D>
#include <QtMath>

#include "CircleEngine.h"
#include "EllipseEngine.h"
#include "PolygonEngine.h"
#include "PolygonFillEngine.h"
#include "StarPolygon.h"
#include "SquarePolygon.h"
#include "HexagonPolygon.h"
#include "Circle.h"
#include "HolePolygon.h"

mainCanvas::mainCanvas(QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);
    //初始化引擎
    this->lineEngine = new LineEngine(this);
    this->polygonFillEngine = new PolygonFillEngine(this);
//    //加入初始图形
    //带洞多边形
    HolePolygon* polygon = new HolePolygon();
    polygon->addVertex(QVector2D(200,200));
    polygon->addVertex(QVector2D(200,500));
    polygon->addVertex(QVector2D(500,500));
    polygon->addVertex(QVector2D(500,200));
    polygon->setFillByScan(true);
    polygons.push_back(polygon);
    //加入洞
    Polygon* polygon2 = new Polygon();
    polygon2->addVertex(QVector2D(300,300));
    polygon2->addVertex(QVector2D(300,400));
    polygon2->addVertex(QVector2D(400,400));
    polygon2->addVertex(QVector2D(400,300));
    polygon->addHole(polygon2);


//    Polygon* polygan=nullptr;
//    polygan = new HexagonPolygon(QVector2D(100,100),QVector2D(200,200),0,this);
//    polygan->setFillByScan(true);
//    this->polygons.push_back(polygan);

//    //四个象限的直线
//    lines.push_back(new Line(QVector2D(200,200),QVector2D(200,300)));
//    lines.push_back(new Line(QVector2D(200,200),QVector2D(300,200)));
//    lines.push_back(new Line(QVector2D(200,200),QVector2D(400,300)));
//    lines.push_back(new Line(QVector2D(200,200),QVector2D(400,100)));
//    lines.push_back(new Line(QVector2D(200,200),QVector2D(300,400)));
//    lines.push_back(new Line(QVector2D(200,200),QVector2D(300,0)));
//    //第一个多边形
//    polygan->addVertex(QVector2D(200,200));
//    polygan->addVertex(QVector2D(400,200));
//    polygan->addVertex(QVector2D(300,400));
//    polygan->addVertex(QVector2D(100,400));
//    this->polygons.push_back(polygan);
//    //第二个多边形
//    polygan = new Polygon(this);
//    polygan->addVertex(QVector2D(200+150,250));
//    polygan->addVertex(QVector2D(500+150,200));
//    polygan->addVertex(QVector2D(300+150,400));
//    polygan->addVertex(QVector2D(150+150,300));
//    this->polygons.push_back(polygan);
//    //加入五角星
//    polygan = new StarPolygon(100,QVector2D(200,200),0,this);
//    polygan->setFillByScan(true);
//    this->polygons.push_back(polygan);
//    //加入正方形
//    polygan = new SquarePolygon(QVector2D(200,200),QVector2D(100,100),0,this);
//    this->polygons.push_back(polygan);
//    //加入六边形
//    polygan = new HexagonPolygon(QVector2D(100,100),QVector2D(200,200),0,this);
//    polygan->setFillByScan(true);
//    this->polygons.push_back(polygan);
//    //加入圆形
//    this->circles.push_back(new Circle(100,QVector2D(100,100)));
//    for(int i=50;i<800;i+=10){
//        this->circles.push_back(new Circle(i,QVector2D(500,400)));
//    }
//    //加入椭圆
//    this->ellipses.push_back(new Ellipse(QVector2D(200,200),100,150));
//    this->ellipses.push_back(new Ellipse(QVector2D(200,200),250,375));
//    for(int i=200;i<800;i+=10){
//        this->ellipses.push_back(new Ellipse(QVector2D(500,400),i,i*1.5));
//    }
}

void mainCanvas::paintEvent(QPaintEvent *event)
{
    //必须在paintEvent中初始化painter，不然会报错
    QPainter painter(this);
    //绘制直线
    if(!lines.empty()){
        for(auto it=lines.begin();it<lines.end();it++){
            Line* line = *it;
            line->draw(&painter,lineEngine);
            if(line->getSelected()){
                //qDebug()<<"line selected!";
                line->drawBox(&painter,lineEngine);
            }
        }
    }
    //绘制多边形
    if(!polygons.empty()){
        PolygonEngine polygonEngine(polygons[0],this);
        for(auto it=polygons.begin();it!=polygons.end();it++){
            //绘制边框
            Polygon* polygon=*it;
            polygon->drawPolygon(&painter,lineEngine);
            //进行填充
            if(polygon->getFillByFence()){
                this->polygonFillEngine->fenceFill(polygon,&painter);
            }
            if(polygon->getFillByScan()){
                this->polygonFillEngine->scanpolygonFill(polygon,&painter);
            }
            if(polygon->getSelected()){
                polygon->drawBoundBox(&painter,lineEngine);
            }
        }
    }
    //绘制圆形
    if(!circles.empty()){
        CircleEngine circleEngine;
        for(auto it=circles.begin();it!=circles.end();it++){
            Circle* circle = *it;
            circle->drawPolygon(&painter,&circleEngine);
        }
    }
    //绘制椭圆
    if(!ellipses.empty()){
        EllipseEngine ellipseEngine;
        for(auto it=ellipses.begin();it!=ellipses.end();it++){
            Ellipse* ellipse = *it;
            ellipse->drawPolygon(&painter,&ellipseEngine);
        }
    }


    //向上传递事件
    QWidget::paintEvent(event);
}

namespace{
    bool drawingPolygon=false;
    bool drawingLine=false;
    bool drawingCircle=false;
    bool drawingEllipse=false;
    bool drawingStar=false;
    bool drawingSquare=false;
    bool drawingHexgon=false;
    bool rotateChanging=false;
    bool scaleChanging=false;
    bool zoomChanging=false;
    bool anchorChanging=false;

    Polygon* selectedPolygon=nullptr; //表示当前选中的多边形
    Line* selectedLine=nullptr;  //当前选中的Line
}


void mainCanvas::mousePressEvent(QMouseEvent *event)
{
    //如果为绘图状态，按下鼠标新建多边形顶点，双击表示结束。
    if(this->tool==ToolType::AddPolygon && drawingPolygon){
        drawPolygon(QVector2D(event->position()),false);
    }else if(this->tool==ToolType::SelectTool){
        //如果为选择工具，单击不需要操作
    }else if(this->tool==ToolType::FillByScan){
        //填充工具，单击时填充选中的多边形
        if(selectedPolygon!=nullptr){
            selectedPolygon->setFillByScan(true);
        }
    }else if(this->tool==ToolType::FillByFence){
        if(selectedPolygon!=nullptr){
            selectedPolygon->setFillByFence(true);
        }
    }
    //绘制图形
    else if(this->tool==ToolType::DrawLine){
        drawLine(QVector2D(event->position()));
        drawingLine=!drawingLine;  //翻转一下
    }else if(this->tool==ToolType::DrawCircle){
        drawCircle(QVector2D(event->position()));
        drawingCircle=true; //开始绘制
    }else if(this->tool==ToolType::DrawEllipse){
        drawEllipse(QVector2D(event->position()));
        drawingEllipse=true;
    }else if(this->tool==ToolType::DrawStar){
        drawStar(QVector2D(event->position()));
        drawingStar=true;
    }else if(this->tool==ToolType::DrawSquare){
        drawSquare(QVector2D(event->position()));
        drawingSquare=true;
    }else if(this->tool==ToolType::DrawHexgon){
        drawHexgon(QVector2D(event->position()));
        drawingHexgon=true;
    }
    //变换工具
    else if(this->tool==ToolType::EditRotate){
        editRotate(QVector2D(event->position()));
        rotateChanging=true;
    }
    else if(this->tool==ToolType::EditScale){
        editScale(QVector2D(event->position()));
        scaleChanging=true;
    }
    else if(this->tool==ToolType::EditZoom){
        editZoom(QVector2D(event->position()));
        zoomChanging=true;
    }
    else if(this->tool==ToolType::EditAnchor){
        editAnchor(QVector2D(event->position()));
        anchorChanging=true;
    }


    update();
    QWidget::mousePressEvent(event);
}

void mainCanvas::mouseDoubleClickEvent(QMouseEvent *event)
{
    //绘制多边形
    if(this->tool==ToolType::AddPolygon){
        //qDebug()<<"void mainCanvas::mouseDoubleClickEvent(QMouseEvent *event)"<<"ToolType::AddPolygon";
        if(!drawingPolygon){
            //开始一次新的绘图
            drawPolygon(QVector2D(event->position()),true);
            drawingPolygon=true;
        }else{
            //结束绘图
            drawingPolygon=false;
        }
    }if(this->tool==ToolType::SelectTool){
    //选择按钮，双击时选中图形
        if(event->button()==Qt::LeftButton){
            qDebug()<<"void mainCanvas::mouseDoubleClickEvent(QMouseEvent *event)"<<"ToolType::SelectTool";
            selectedPolygon=selectPolygon(QVector2D(event->position()));
            selectedLine=selectLine(QVector2D(event->position()));
        }
    }
    update();
    QWidget::mouseDoubleClickEvent(event);
}

void mainCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if(this->tool==ToolType::AddPolygon){

    }else if(event->buttons() & Qt::LeftButton && this->tool==ToolType::SelectTool && selectedPolygon!=nullptr){
        //选择按钮，鼠标移动时更新正在选中的Polygon的位置属性
        //qDebug()<<"void mainCanvas::mouseMoveEvent(QMouseEvent *event)"<<"tool==ToolType::SelectTool";
        selectedPolygon->setPosition(QVector2D(event->pos()));
    }
    //绘制
    else if(this->tool==ToolType::DrawLine && drawingLine==true){
        drawLine(QVector2D(event->position()));
        update();
    }else if(event->buttons() & Qt::LeftButton && this->tool==ToolType::DrawCircle && drawingCircle==true){
        drawCircle(QVector2D(event->position()));
    }else if(event->buttons() & Qt::LeftButton && this->tool==ToolType::DrawEllipse && drawingEllipse==true){
        drawEllipse(QVector2D(event->position()));
    }else if(event->buttons() & Qt::LeftButton && this->tool==ToolType::DrawStar && drawingStar==true){
        drawStar(QVector2D(event->position()));
    }else if(event->buttons() & Qt::LeftButton && this->tool==ToolType::DrawSquare && drawingSquare==true){
        drawSquare(QVector2D(event->position()));
    }
    else if(event->buttons() & Qt::LeftButton && this->tool==ToolType::DrawHexgon && drawingHexgon==true){
        drawHexgon(QVector2D(event->position()));
    }
    //变换工具
    else if(event->buttons() & Qt::LeftButton && this->tool==ToolType::EditRotate && rotateChanging==true){
        editRotate(QVector2D(event->position()));
    }
    else if(event->buttons() & Qt::LeftButton && this->tool==ToolType::EditScale && scaleChanging==true){
        editScale(QVector2D(event->position()));
    }
    else if(event->buttons() & Qt::LeftButton && this->tool==ToolType::EditZoom && zoomChanging==true){
        editZoom(QVector2D(event->position()));
    }
    else if(event->buttons() & Qt::LeftButton && this->tool==ToolType::EditAnchor && anchorChanging==true){
        editAnchor(QVector2D(event->position()));
    }
    if(event->buttons() & Qt::LeftButton){
        update();
    }

    QWidget::mouseMoveEvent(event);
}

void mainCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    //圆形
    if(event->button()==Qt::LeftButton && drawingCircle){
        drawCircle(QVector2D(event->position()));
        drawingCircle=false;
    }
    //椭圆
    if(event->button()==Qt::LeftButton && drawingEllipse){
        drawEllipse(QVector2D(event->position()));
        drawingEllipse=false;
    }
    //五角星
    if(event->button()==Qt::LeftButton && drawingStar){
        drawStar(QVector2D(event->position()));
        drawingStar=false;
    }
    //正方形
    if(event->button()==Qt::LeftButton && drawingSquare){
        drawSquare(QVector2D(event->position()));
        drawingSquare=false;
    }
    //五角星
    if(event->button()==Qt::LeftButton && drawingHexgon){
        drawHexgon(QVector2D(event->position()));
        drawingHexgon=false;
    }
    //变换
    if(event->button()==Qt::LeftButton && rotateChanging){
        editRotate(QVector2D(event->position()));
        rotateChanging=false;
    }
    if(event->button()==Qt::LeftButton && scaleChanging){
        editScale(QVector2D(event->position()));
        scaleChanging=false;
    }
    if(event->button()==Qt::LeftButton && zoomChanging){
        editZoom(QVector2D(event->position()));
        zoomChanging=false;
    }
    if(event->button()==Qt::LeftButton && anchorChanging){
        editAnchor(QVector2D(event->position()));
        anchorChanging=false;
    }

    update();

    QWidget::mouseReleaseEvent(event);
}

void mainCanvas::drawPolygon(QVector2D pos, bool newPolygon)
{
    if(newPolygon){
        this->polygons.push_back(new Polygon(this));
    }
    polygons.back()->addVertex(pos);
    qDebug()<<"draw finished";
    //update();
}

Polygon* mainCanvas::selectPolygon(QVector2D pos){
    //用包围盒判断选中哪一个，如果有多个都可以选中，用包围盒面积判断
    //释放上一次选中
    if(selectedPolygon!=nullptr){
        selectedPolygon->setSelected(false);
    }
    Polygon* temp = nullptr;
    int Area=0;
    for(auto it=this->polygons.begin(); it!=polygons.end();it++){
        Boundbox box = (*it)->generateBoundBox();
        //qDebug()<<"bound box";
        qDebug()<<box.getXMax()<<box.getXMin()<<box.getYMax()<<box.getYMin();
        if(box.getXMax()>pos.x() && box.getXMin()<pos.x() && box.getYMax()>pos.y() && box.getYMin()<pos.y()){
            //点击区域在BoundBox内部
            if(Area<(box.getXMax()-box.getXMin())*(box.getYMax()-box.getYMin())){
                Area = (box.getXMax()-box.getXMin())*(box.getYMax()-box.getYMin());
                temp = *it;
            }
        }
    }
    if(temp!=nullptr){
        temp->setSelected(true);
    }
    return temp;
}

Line *mainCanvas::selectLine(QVector2D pos)
{
    //用包围盒判断选中哪一个，选中第一个
    Line* temp = nullptr;
    for(auto it=this->lines.begin(); it!=lines.end();it++){
        Line::BoundBox box = (*it)->getBox();
        if(box.inBox(pos)){
            (*it)->setSelected(true);
            return *it;
        }
    }
    //qDebug()<<"no line selected";
    return temp;
}

void mainCanvas::drawLine(QVector2D pos)
{
    if(!drawingLine){
        Line* line = new Line(pos,pos,this);
        lines.push_back(line);
        return ;
    }else{
        Line* line=lines.back();
        line->setEnd(pos);
    }
}

namespace{
QVector2D circleStart;
}
void mainCanvas::drawCircle(QVector2D pos)
{
    if(!drawingCircle){
        circles.push_back(new Circle(0,pos));
        circleStart=pos;
    }else{
        Circle* circle=circles.back();
        circle->setRadians(circleStart.distanceToPoint(pos));
    }
}
namespace{
QVector2D ellipseStart;
}
void mainCanvas::drawEllipse(QVector2D pos)
{
    if(!drawingEllipse){
        ellipses.push_back(new Ellipse(pos,0,0));
        ellipseStart=pos;
    }else{
        Ellipse* ellipse=ellipses.back();
        ellipse->setA(abs(ellipseStart.x()-pos.x()));
        ellipse->setB(abs(ellipseStart.y()-pos.y()));
    }
}

namespace{
QVector2D starStart;
}
void mainCanvas::drawStar(QVector2D pos)
{
    if(!drawingStar){
        polygons.push_back(new StarPolygon(0,pos));
        starStart=pos;
    }else{
        Polygon* polygon = polygons.back();
        polygon->setScale(pos.distanceToPoint(starStart));
        qDebug()<<polygon->getScale();
    }
}

namespace{
QVector2D squareStart;
}
void mainCanvas::drawSquare(QVector2D pos)
{
    if(!drawingSquare){
        squareStart=pos;
        polygons.push_back(new SquarePolygon(pos,QVector2D(0,0),0,this));

    }else{
        Polygon* polygon = polygons.back();
        polygon->setScale(pos.distanceToPoint(squareStart));
        qDebug()<<polygon->getScale();
    }
}

namespace{
QVector2D hexgonStart;
}
void mainCanvas::drawHexgon(QVector2D pos)
{
    if(!drawingHexgon){
        hexgonStart=pos;
        polygons.push_back(new HexagonPolygon(QVector2D(0,0),pos,0,this));

    }else{
        Polygon* polygon = polygons.back();
        polygon->setScale(pos.distanceToPoint(hexgonStart));
        qDebug()<<polygon->getScale();
    }
}
namespace{
QVector2D rotateStart;
QVector2D scaleStart;
QVector2D zoomStart;
QVector2D anchorStart;

}
void mainCanvas::editRotate(QVector2D pos)
{
    if(!rotateChanging){
        rotateStart = pos; //记录pos
    }else{
        QVector2D center = selectedPolygon ->generateBoundBox().getCenter();
        QVector2D vector1=pos-center,vector2=rotateStart-center;
        // 计算向量的点积
        qreal dotProduct = QVector2D::dotProduct(vector1, vector2);
        // 计算向量的模
        qreal length1 = vector1.length();
        qreal length2 = vector2.length();
        // 计算夹角（弧度）
        qreal angleInRadians = qAcos(fminl(fmaxl(dotProduct / (length1 * length2),-1.0),1.0));
        // 将弧度转换为角度
        qreal angleInDegrees = qRadiansToDegrees(angleInRadians);
        selectedPolygon->addAngle((float)angleInDegrees);
        rotateStart = pos;
        qDebug()<<"angle="<<angleInDegrees<<angleInRadians<<"angle="<<selectedPolygon->getAngle();

    }
}

void mainCanvas::editScale(QVector2D pos)
{
    //编辑缩放
    if(!scaleChanging){
        scaleStart = pos; //记录pos
    }else{
        QVector2D offset = scaleStart-pos;
        offset.setX(-offset.x());
        //offset转化为比例
        Boundbox box = selectedPolygon->generateBoundBox();
        QVector2D size = QVector2D(box.getXMax()-box.getXMin(),box.getYMax()-box.getYMin());
        QVector2D scale = (offset+size)/size;
        qDebug()<<"scale="<<scale;
        selectedPolygon->addScale(scale);
        scaleStart = pos;
    }
}

void mainCanvas::editZoom(QVector2D pos)
{
    //编辑错切
    qDebug()<<"scale edif";
    if(!zoomChanging){
        zoomStart = pos; //记录pos
    }else{
        QVector2D offset = zoomStart-pos;
        offset.setX(-offset.x());
        //offset转化为比例
        Boundbox box = selectedPolygon->generateBoundBox();
        QVector2D size = QVector2D(box.getXMax()-box.getXMin(),box.getYMax()-box.getYMin());
        QVector2D scale = (offset)/size;

        qDebug()<<"scale="<<scale;
        selectedPolygon->addZoom(scale);
        zoomStart = pos;
    }

    //update();
}

void mainCanvas::editAnchor(QVector2D pos)
{
    qDebug()<<"void mainCanvas::editAnchor(QVector2D pos)";
}



void mainCanvas::setTool(ToolType type)
{
    qDebug()<<"tool="<<type;
    this->tool = type;
}

void mainCanvas::setFillColor(QVector4D color)
{
    this->polygonFillEngine->setColor(color);
}

void mainCanvas::setTransform(int rotate, QVector2D scale, QVector2D position, QVector2D zoom)
{
    qDebug()<<"mainCanvas::setTransform"<<rotate<<scale<<position<<zoom;
    if(selectedPolygon==nullptr){
        return;
    }
    selectedPolygon->setAngle(rotate);
    selectedPolygon->setScale(scale.x(),scale.y());
    selectedPolygon->setPosition(position);
    selectedPolygon->setZoom(zoom.x(),zoom.y());

    //update();
}

void mainCanvas::setAttrib(QVector2D circleCenter, QVector2D radian, QVector2D ellpse, int squareSize, int starSize, int hexgonSize)
{
    qDebug()<<"mainCanvas::setAttrib";
    if(dynamic_cast<StarPolygon*>(selectedPolygon)!=nullptr){
        StarPolygon* star = dynamic_cast<StarPolygon*>(selectedPolygon);
        if(starSize!=0){
            star->setScale(starSize);
        }else{
            qDebug()<<"invalid input";
        }
    }
    else if(dynamic_cast<SquarePolygon*>(selectedPolygon)!=nullptr){
        SquarePolygon* square =  dynamic_cast<SquarePolygon*>(selectedPolygon);
        if(squareSize!=0){
            square->setEdgeLength(squareSize);
        }else{
            qDebug()<<"invalid input";
        }
    }else if(dynamic_cast<HexagonPolygon*>(selectedPolygon)!=nullptr){
        HexagonPolygon* hexgon =  dynamic_cast<HexagonPolygon*>(selectedPolygon);
        if(hexgonSize!=0){
            hexgon->setEdgeLength(hexgonSize);
        }else{
            qDebug()<<"invalid input";
        }
    }
    update();
}

void mainCanvas::deletePolygonSlot()
{
    if(selectedPolygon!=nullptr){
        polygons.removeAll(selectedPolygon);
        //delete selectedPolygon;
        selectedPolygon=nullptr;
    }
    if(selectedLine!=nullptr){
        selectedLine->setSelected(false);
        lines.removeAll(selectedLine);
        //delete selectedLine;
        selectedLine=nullptr;
    }

    update();
}

void mainCanvas::deleteAllPolygonSlot()
{
    polygons.clear();
    lines.clear();
    circles.clear();
    ellipses.clear();

    update();
}

void mainCanvas::cancleFillSlot()
{
    selectedPolygon->setFillByFence(false);
    selectedPolygon->setFillByScan(false);

    update();
}

