#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector2D>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    void initUi();
    void initSignal();
signals:
    void updateTransform(int rotate, QVector2D scale, QVector2D position, QVector2D zoom);  //接受一组表示图形变换的参数，将参数传递给mainVanvas
    void setColor(QVector4D color);
public slots:
    void setTool(QAction* action);  // 根据选中的QAction，切换mainCanvas中的Tool
    void setColorDialog();  //弹出颜色选择对话框
};
#endif // MAINWINDOW_H
