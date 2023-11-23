#ifndef ELLIPSECONTROLWIDGET_H
#define ELLIPSECONTROLWIDGET_H

#include <QWidget>
#include <QVector2D>

namespace Ui {
class EllipseControlWidget;
}

class EllipseControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EllipseControlWidget(QWidget *parent = nullptr);
    ~EllipseControlWidget();

private:
    Ui::EllipseControlWidget *ui;

signals:
    void updateAttrib(QVector2D circleCenter,QVector2D radian, QVector2D ellpse, int squareSize ,int starSize,int hexgonSize);

public slots:
    void attribUpdate();   //确定按钮点击后触发，出发后采集Label中信息，发送到mianCanve
};

#endif // ELLIPSECONTROLWIDGET_H
