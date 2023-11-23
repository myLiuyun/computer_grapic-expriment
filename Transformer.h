/*
提供一个表示变换的类，维护一个矩阵，表示变换矩阵，并提供各种变换的方法
*/
#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <QObject>
#include <QGenericMatrix>

class Transformer : public QObject
{
    Q_OBJECT
public:
    explicit Transformer(QObject *parent = nullptr);


private:

    QMatrix3x3 transform;

signals:

public slots:
    QMatrix3x3 getTransform();
    void addMovement(const QVector2D& pos);
    void addRotation(const QVector2D& anchor, float angle); //分别为锚点坐标。旋转角度，角度制
    void addScale(const QVector2D& anchor,float x, float y);  //表示在x和y方向缩减的系数
    void addScale(const QVector2D& anchor,float k);
};

#endif // TRANSFORMER_H
