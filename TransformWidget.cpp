#include "TransformWidget.h"
#include "ui_TransformWidget.h"

TransformWidget::TransformWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransformWidget)
{
    ui->setupUi(this);
}

TransformWidget::~TransformWidget()
{
    delete ui;
}

void TransformWidget::reciveData()
{
    qDebug()<<"void TransformWidget::reciveData()";
    float angle = ui->rotateEdit->text().toInt();
    QVector2D position = QVector2D(ui->positionXEdit->text().toInt(), ui->positionYEdit->text().toInt());
    QVector2D scale = QVector2D(ui->scaleXEdit->text().toFloat(),ui->scaleYEdit->text().toFloat());
    QVector2D zoom = QVector2D(ui->zoomXEdit->text().toFloat(),ui->zoomYEdit->text().toFloat());
    qDebug()<<"zoom="<<zoom;
    emit sendData(angle,scale,position,zoom);
}

void TransformWidget::cancleFill()
{
    emit cancleFillSignal();
}

void TransformWidget::deletePolygon()
{
    emit deletePolygonSignal();
}

void TransformWidget::deleteAllPolygon()
{
    emit deleteAllPolygonSignal();
}


