/*
主要的画布类
所有绘制的结果展示在这里
*/

#ifndef MAINCANVAS_H
#define MAINCANVAS_H

#include <QWidget>
#include <QPointF>

#include "LineEngine.h"
#include "PolygonFillEngine.h"

#include "Polygon.h"
#include "Line.h"
#include "Circle.h"
#include "Ellipse.h"

class mainCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit mainCanvas(QWidget *parent = nullptr);
    enum ToolType{
        SelectTool = 0,
        AddPolygon = 1,
        FillByScan = 2,
        FillByFence = 3,
        DrawLine = 4,
        DrawCircle = 5,
        DrawEllipse = 6,
        DrawStar = 7,
        DrawSquare = 8,
        DrawHexgon=9,
        EditRotate=10, //旋转工具
        EditScale=11, //缩放工具
        EditZoom=12, //错切工具
        EditAnchor=13 //锚点工具

    };
    QVector<Polygon*> polygons;  //保存所有多边形对象
    QVector<Line*> lines;
    QVector<Circle*> circles;
    QVector<Ellipse*> ellipses;
protected:

    ToolType tool=SelectTool;  //标记现在的Tool类型，被QAction改变
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    LineEngine* lineEngine=nullptr;
    PolygonFillEngine* polygonFillEngine=nullptr;

    void drawPolygon(QVector2D pos, bool newPolygon);  //新建多边形的函数，根据newPolygon判断是否需要新建多边形
    Polygon* selectPolygon(QVector2D pos);   //选择工具单击时调用，根据单击的位置，选中对应的多边形
    Line* selectLine(QVector2D pos);
    void drawLine(QVector2D pos);
    void drawCircle(QVector2D pos);
    void drawEllipse(QVector2D pos);
    void drawStar(QVector2D pos);
    void drawSquare(QVector2D pos);
    void drawHexgon(QVector2D pos);
    void editRotate(QVector2D pos);
    void editScale(QVector2D pos);
    void editZoom(QVector2D pos);
    void editAnchor(QVector2D pos);
signals:


public slots:
    //void selectPolygon(QVector2D pos);    //根据点击的位置，选择一个Polygon
    void setTool(ToolType type);
    void setFillColor(QVector4D color);
    void setTransform(int rotate, QVector2D scale, QVector2D position, QVector2D zoom);  //使用接受的参数，更新选中中的Polygon
    void setAttrib(QVector2D circleCenter,QVector2D radian, QVector2D ellpse, int squareSize ,int starSize,int hexgonSize);
    void deletePolygonSlot();
    void deleteAllPolygonSlot();
    void cancleFillSlot();
};

#endif // MAINCANVAS_H
