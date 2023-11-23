#include "EllipseControlWidget.h"
#include "ui_EllipseControlWidget.h"

#include <QIntValidator> // 验证QString到int

EllipseControlWidget::EllipseControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EllipseControlWidget)
{
    ui->setupUi(this);
}

EllipseControlWidget::~EllipseControlWidget()
{
    delete ui;
}

void EllipseControlWidget::attribUpdate(){
    qDebug()<<"EllipseControlWidget::attribUpdate";
    QIntValidator validator;

    int circleCenerX=0,circleCenerY=0,ellipseA=0,ellipseB=0,radiansX=0,radiansY=0;
    int squareLength=0,starSize=0,hexgonSize=0;

     circleCenerX = ui->circleCenterX->text().toInt();
     circleCenerY = ui->circleCenterY->text().toInt();
    QVector2D circleCenter = QVector2D(circleCenerX,circleCenerY);

     ellipseA = ui->ellplseA->text().toInt();  ellipseB = ui->ellipseB->text().toInt();
    QVector2D ellipseLength = QVector2D(ellipseA,ellipseB);

     radiansX = ui->radiansX->text().toInt();  radiansY = ui->radiansY->text().toInt();
    QVector2D radians = QVector2D(radiansX,radiansY);

     squareLength = ui->squareLength->text().toInt();
     starSize = ui->starLength->text().toInt();
     hexgonSize = ui->hexgonLength->text().toInt();

    emit updateAttrib(circleCenter,radians,ellipseLength,squareLength,starSize,hexgonSize);
}
