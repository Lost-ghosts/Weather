#ifndef MAINWINDOW_H

#define MAINWINDOW_H
#include <QObject>
#include <QMainWindow>
#include <QWidget>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QLabel>
#include <QList>
#include <QNetworkAccessManager>
#include "weathertool.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#include <QPainter>
#include <QTimer>
#include "weatherdata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    virtual bool eventFilter(QObject *watched, QEvent *event);

protected:

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void getWeatherInfo(QNetworkAccessManager *manager);
    void parseJson(QByteArray& bytes);
    void setLabelContent();
    void paintSunRiseSet();
    void paintCurve();
private slots:
    void slot_exitApp();
    void replayFinished(QNetworkReply* reply);
    void on_refreshBt_clicked();

    void on_searchBt_clicked();

private:
    Ui::MainWindow *ui;
    QMenu *exitMenu;    //右键退出的菜单
    QAction *exitAct;   //退出的行为
    QPoint mPos;        //窗口移动需要的点
    //ui列表
    QList<QLabel *> forecast_week_list;         //星期
    QList<QLabel *> forecast_date_list;         //日期
    QList<QLabel *> forecast_aqi_list;          //天气指数
    QList<QLabel *> forecast_type_list;         //天气
    QList<QLabel *> forecast_typeIco_list;      //天气图标
    QList<QLabel *> forecast_high_list;         //高温
    QList<QLabel *> forecast_low_list;          //低温

    QString url;            //接口连接
    QString city;           //访问的城市
    QString cityTmp;        //临时存放城市变量,防止输入错误城市的时候,原来的城市名称还在。
    WeatherTool tool;       //天气工具对象

    QNetworkAccessManager* manager;

    //本地数据
    Today today;
    Forecast forecast[6];

    //
    static const QPoint sun[2];             //日出日落底线
    static const QRect sunRizeSet[2];       //日出日落时间
    static const QRect rect[2];             //日出日落圆弧

    QTimer* sunTimer;

};
#endif // MAINWINDOW_H
