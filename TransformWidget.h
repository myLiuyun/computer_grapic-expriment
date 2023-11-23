#ifndef TRANSFORMWIDGET_H
#define TRANSFORMWIDGET_H

#include <QWidget>
#include <QVector2D>

namespace Ui {
class TransformWidget;
}

class TransformWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransformWidget(QWidget *parent = nullptr);
    ~TransformWidget();

private:
    Ui::TransformWidget *ui;

signals:
    void sendData(int rotate, QVector2D scale, QVector2D position, QVector2D zoom); // 数据传递到MainWiindow
    void cancleFillSignal();
    void deletePolygonSignal();
    void deleteAllPolygonSignal();

public slots:
    void reciveData();  //确定按钮点击后，收集一次数据，
    void cancleFill();
    void deletePolygon();
    void deleteAllPolygon();
};

#endif // TRANSFORMWIDGET_H
