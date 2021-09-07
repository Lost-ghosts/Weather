#ifndef MAINWINDOW_H

#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QPoint>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
private slots:
    void slot_exitApp();
private:
    Ui::MainWindow *ui;
    QMenu *exitMenu;    //右键退出的菜单
    QAction *exitAct;   //退出的行为
    QPoint mPos;        //窗口移动需要的点
};
#endif // MAINWINDOW_H
