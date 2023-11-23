#include <QColorDialog>

#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "QActionGroup"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initUi();
    this->initSignal();



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initUi()
{
    QActionGroup* toolGroup = new QActionGroup(this);
    //绘图
    toolGroup->addAction(this->ui->drawLine);
    toolGroup->addAction(this->ui->drawCircle);
    toolGroup->addAction(this->ui->drawEllipse);
    toolGroup->addAction(this->ui->drawStar);
    toolGroup->addAction(this->ui->drawSquare);
    toolGroup->addAction(this->ui->drawHexgon);
    toolGroup->addAction(this->ui->drawPolygon);

    //填充
    toolGroup->addAction(this->ui->fillPolygon);
    toolGroup->addAction(this->ui->fillFence);

    //变换
    toolGroup->addAction(this->ui->selectTool);
    toolGroup->addAction(this->ui->shearConversion);
    toolGroup->addAction(this->ui->rotateConversion);
    toolGroup->addAction(this->ui->scaleConversion);
    toolGroup->addAction(this->ui->anchorConversion);

    //设置为单选
    toolGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);

    //信号和槽
    connect(toolGroup,&QActionGroup::triggered,this,&MainWindow::setTool);
}

void MainWindow::initSignal()
{

}

void MainWindow::setTool(QAction *action)
{
    //根据不同的QAction，切换工具
    qDebug()<<"void MainWindow::setTool(QAction *action)";
    if(action==this->ui->selectTool){
        this->ui->canvas->setTool(mainCanvas::SelectTool);
    }else if(action==this->ui->drawPolygon){
        qDebug()<<"set tool mainCanvas::AddPolygon";
        this->ui->canvas->setTool(mainCanvas::AddPolygon);
    }else if(action==this->ui->fillPolygon){
        qDebug()<<"set tool mainCanvas::fillByScan";
        this->ui->canvas->setTool(mainCanvas::FillByScan);
    }else if(action==this->ui->fillFence){
        qDebug()<<"set tool mainCanvas::fillByFence";
        this->ui->canvas->setTool(mainCanvas::FillByFence);
    }else if(action==this->ui->drawLine){
        qDebug()<<"set tool mainCanvas::DrawLine";
        this->ui->canvas->setTool(mainCanvas::DrawLine);
    }else if(action==this->ui->drawCircle){
        qDebug()<<"set tool mainCanvas::DrawCircle";
        this->ui->canvas->setTool(mainCanvas::DrawCircle);
    }else if(action==this->ui->drawEllipse){
        qDebug()<<"set tool mainCanvas::DrawEllipse";
        this->ui->canvas->setTool(mainCanvas::DrawEllipse);
    }else if(action==this->ui->drawStar){
        qDebug()<<"set tool mainCanvas::DrawStar";
        this->ui->canvas->setTool(mainCanvas::DrawStar);
    }else if(action==this->ui->drawSquare){
        qDebug()<<"set tool mainCanvas::DrawSquare";
        this->ui->canvas->setTool(mainCanvas::DrawSquare);
    }else if(action==this->ui->drawHexgon){
        qDebug()<<"set tool mainCanvas::DrawHexgon";
        this->ui->canvas->setTool(mainCanvas::DrawHexgon);
    }
    else if(action==this->ui->rotateConversion){
        this->ui->canvas->setTool(mainCanvas::EditRotate);
    }
    else if(action==this->ui->scaleConversion){
        this->ui->canvas->setTool(mainCanvas::EditScale);
    }
    else if(action==this->ui->shearConversion){
        this->ui->canvas->setTool(mainCanvas::EditZoom);
    }
    else if(action==this->ui->anchorConversion){
        this->ui->canvas->setTool(mainCanvas::EditAnchor);
    }
}

void MainWindow::setColorDialog()
{
    QColorDialog dialog(Qt::red, this);
    dialog.setOption(QColorDialog::ShowAlphaChannel);
    dialog.exec();
    QColor color = dialog.currentColor();
    emit setColor(QVector4D(color.red(), color.green(), color.blue(), color.alpha()));
}









