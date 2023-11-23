#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QVector2D>
#include <QMatrix3x3>

#include "LineEngine.h"

/*
Line的接口保证Start的X坐标小于End的X坐标
*/

class Line : public QObject
{
    Q_OBJECT
public:
    struct BoundBox{
        /*以Qt坐标系为基准*/
        QVector2D leftUp;
        QVector2D leftDown;
        QVector2D rightUp;
        QVector2D rightDown;
        bool inBox(QVector2D pos);
    };

    Line(QObject *parent = nullptr);
    Line(QVector2D start, QVector2D end, QObject *parent = nullptr);

    void setStart(QVector2D pos);
    void setEnd(QVector2D pos);
    void setSelected(bool state);
    bool getSelected();
    QVector2D getStart();  //获取变换后的坐标
    QVector2D getEnd();
    Line::BoundBox getBox();

    void draw(QPainter* painter,LineEngine* engine);
    void drawBox(QPainter* painter, LineEngine* engine);

private:
    QVector2D start=QVector2D(-1,-1);
    QVector2D end=QVector2D(-1,-1);

    bool selected=false; //是否被选中
    float rotote=0.0f;
    QVector2D position=QVector2D(0.0f,0.0f);
    QVector2D scale=QVector2D(1.0f,1.0f);
    QVector2D zoom=QVector2D(0.0f,0.0f);

    QMatrix3x3 getTransform();  //获取变换矩阵
    QVector2D transform(QMatrix3x3 transform, QVector2D point);
    QMatrix3x3 invertedMatrix(QMatrix3x3 mat);
signals:

};

#endif // LINE_H
