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
    QMenu *exitMenu;    //�Ҽ��˳��Ĳ˵�
    QAction *exitAct;   //�˳�����Ϊ
    QPoint mPos;        //�����ƶ���Ҫ�ĵ�
    //ui�б�
    QList<QLabel *> forecast_week_list;         //����
    QList<QLabel *> forecast_date_list;         //����
    QList<QLabel *> forecast_aqi_list;          //����ָ��
    QList<QLabel *> forecast_type_list;         //����
    QList<QLabel *> forecast_typeIco_list;      //����ͼ��
    QList<QLabel *> forecast_high_list;         //����
    QList<QLabel *> forecast_low_list;          //����

    QString url;            //�ӿ�����
    QString city;           //���ʵĳ���
    QString cityTmp;        //��ʱ��ų��б���,��ֹ���������е�ʱ��,ԭ���ĳ������ƻ��ڡ�
    WeatherTool tool;       //�������߶���

    QNetworkAccessManager* manager;

    //��������
    Today today;
    Forecast forecast[6];

    //
    static const QPoint sun[2];             //�ճ��������
    static const QRect sunRizeSet[2];       //�ճ�����ʱ��
    static const QRect rect[2];             //�ճ�����Բ��

    QTimer* sunTimer;

};
#endif // MAINWINDOW_H
